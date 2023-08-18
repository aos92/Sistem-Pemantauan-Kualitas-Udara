#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "android";
const char* password = "1234567890";

AsyncWebServer server(80);

int buz = 2;
int fan = 4;
int greenled = 5;
int redled = 18;

int gas_sensor = 34;
float m = -0.353;
float c = 0.711;
float R0 = 23.30;

int CO_sensor = 35;
float m1 = -0.67;
float c1 = 1.34;
float R01 = 5.80;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.print("Sistem Pemantauan");
  lcd.setCursor(0, 1);
  lcd.print("Polusi Udara");
  delay(4000);
  lcd.clear();
  
  pinMode(buz, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(gas_sensor, INPUT);
  pinMode(CO_sensor, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<!DOCTYPE html><html><head><title>Pemantauan Udara</title>";
    html += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css'>";
    html += "<style>";
    html += "body { font-size: 16px; }";
    html += ".large-text { font-size: 48px; }";
    html += ".clean-air { border: 5px solid green; padding: 10px; }";
    html += ".dirty-air { border: 5px solid red; padding: 10px; }";
    html += "</style>";
    html += "</head><body><div class='container'>";
    html += "<h1 class='mt-3'>Pemantauan Kualitas Udara</h1>";
    html += "<div class='row'>";
    html += "<div class='col-md-6 mt-3'>";
    html += "<h2>CO2 PPM: <span id='co2Value' class='large-text'></span></h2>";
    html += "<h2>CO PPM: <span id='coValue' class='large-text'></span></h2>";
    html += "</div>";
    html += "<div class='col-md-6 mt-3'>";
    html += "<div id='airStatus' class='clean-air'>";
    html += "<h3>Status Udara: Bersih</h3>";
    html += "</div>";
    html += "</div>";
    html += "</div>";
    html += "<canvas id='chart' width='400' height='200'></canvas>";
    html += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";
    html += "<script>";
    html += "var ctx = document.getElementById('chart').getContext('2d');";
    html += "var chart = new Chart(ctx, {";
    html += "  type: 'line',";
    html += "  data: {";
    html += "    labels: [],";
    html += "    datasets: [{";
    html += "      label: 'CO2 PPM',";
    html += "      data: [],";
    html += "      borderColor: 'blue',";
    html += "      fill: false";
    html += "    }, {";
    html += "      label: 'CO PPM',";
    html += "      data: [],";
    html += "      borderColor: 'orange',";
    html += "      fill: false";
    html += "    }]";
    html += "  }";
    html += "});";
    html += "setInterval(function() {getData();}, 5000);";
    html += "function getData() {fetch('/data').then(response => response.json()).then(data => {";
    html += "  document.getElementById('co2Value').textContent = data.co2;";
    html += "  document.getElementById('coValue').textContent = data.co;";
    html += "  var airStatusElement = document.getElementById('airStatus');";
    html += "  airStatusElement.textContent = 'Status Udara: ' + data.status;";
    html += "  if (data.status === 'Udara Kotor') { airStatusElement.className = 'dirty-air'; }";
    html += "  else { airStatusElement.className = 'clean-air'; }";
    html += "  chart.data.labels.push(new Date().toLocaleTimeString());";
    html += "  chart.data.datasets[0].data.push(data.co2);";
    html += "  chart.data.datasets[1].data.push(data.co);";
    html += "  chart.update();";
    html += "});}";
    html += "</script>";
    html += "</div></body></html>";
    request->send(200, "text/html", html);
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    float ppm = calculatePPM(analogRead(gas_sensor), R0, m, c);
    float ppm1 = calculatePPM(analogRead(CO_sensor), R01, m1, c1);
    String status = (ppm >= 10 || ppm1 >= 10) ? "Udara Kotor" : "Udara Normal";
    String json = "{\"co2\":" + String(ppm) + ",\"co\":" + String(ppm1) + ",\"status\":\"" + status + "\"}";
    request->send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  float ppm = calculatePPM(analogRead(gas_sensor), R0, m, c);
  float ppm1 = calculatePPM(analogRead(CO_sensor), R01, m1, c1);
  
  Serial.print("CO2 PPM: ");
  Serial.println(ppm);
  Serial.print("CO PPM: ");
  Serial.println(ppm1);

  if (ppm >= 10 || ppm1 >= 10) {
    digitalWrite(greenled, LOW);
    digitalWrite(buz, HIGH);
    digitalWrite(redled, HIGH);
    digitalWrite(fan, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Udara Kotor !!");
    Serial.println("Bahaya !!!");
    delay(2000);
    lcd.clear();
  } else {
    digitalWrite(greenled, HIGH);
    digitalWrite(redled, LOW);
    digitalWrite(buz, LOW);
    digitalWrite(fan, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Udara Normal !!");
    Serial.println("Normal !!!");
    delay(2000);
    lcd.clear();
  }
  delay(1000);
}

float calculatePPM(int sensorValue, float R0, float m, float c) {
  float sensor_volt = sensorValue * (3.3 / 4096.0);
  float RS_gas = ((3.3 * 10.0) / sensor_volt) - 10.0;
  float ratio = RS_gas / R0;
  double ppm_log = (log10(ratio) - c) / m;
  double ppm = pow(10, ppm_log);
  return ppm;
}
        
