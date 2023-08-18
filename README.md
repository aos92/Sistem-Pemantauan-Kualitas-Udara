# Sistem Pemantauan Kualitas Udara

## Pendahuluan
Proyek ini adalah sistem pemantauan kualitas udara yang menggunakan perangkat ESP32 untuk mengukur tingkat CO2 dan CO dalam udara. Data yang diukur kemudian ditampilkan dalam bentuk grafik pada halaman web yang dapat diakses melalui jaringan WiFi.

## Komponen
- ESP32
- Modul LCD I2C (LiquidCrystal_I2C)
- Modul Sensor Gas (MQ-135) untuk CO2
- Modul Sensor Gas (MQ-7) untuk CO
- LED Hijau (indikator udara bersih)
- LED Merah (indikator udara kotor)
- Buzzer (untuk peringatan)
- Kipas (untuk sirkulasi udara)
- Breadboard dan kabel-kabel penghubung

## Koneksi Komponen
- Modul LCD dihubungkan melalui I2C (alamat 0x27)
- Pin sensor gas CO2 dihubungkan ke pin 34
- Pin sensor gas CO dihubungkan ke pin 35
- LED hijau dihubungkan ke pin 5
- LED merah dihubungkan ke pin 18
- Buzzer dihubungkan ke pin 2
- Kipas dihubungkan ke pin 4

## Algoritma
1. Pada saat setup, modul LCD akan menampilkan pesan "Sistem Pemantauan Polusi Udara" dan berubah setelah 4 detik.
2. Modul WiFi akan terhubung ke jaringan WiFi yang telah ditentukan.
3. Web server dijalankan pada port 80 dan memberikan akses ke halaman pemantauan.
4. Pada halaman web, digunakan Chart.js untuk menampilkan grafik tingkat CO2 dan CO dari waktu ke waktu.
5. Data dari sensor gas CO2 dan CO akan diambil setiap 5 detik dan ditampilkan pada halaman web.
6. Status kualitas udara (bersih atau kotor) akan dihitung berdasarkan ambang batas tertentu.
7. LED hijau atau merah akan menyala sesuai dengan status udara.
8. Buzzer akan berbunyi jika kualitas udara melewati ambang batas.

## Cara Menggunakan untuk Akses Web
1. Hubungkan komponen sesuai dengan koneksi yang telah dijelaskan.
2. Upload skrip ke perangkat Arduino atau ESP8266.
3. Pastikan perangkat terhubung ke jaringan WiFi yang sesuai.
4. Buka browser dan akses alamat IP perangkat di jaringan (biasanya ditampilkan di serial monitor setelah perangkat terhubung ke WiFi).
5. Anda akan melihat halaman pemantauan kualitas udara yang menampilkan grafik dan status udara.


# Penggunaan persamaan:

## Tahap 1: Hukum Ohm
Ini adalah persamaan dasar dari Hukum Ohm yang menghubungkan tegangan (V), arus (I), dan resistansi (R).

```
I = V/R
```

## Tahap 2: Tegangan Output pada Resistor Beban
Persamaan ini menghubungkan tegangan output (VRl) pada resistor beban (RL) dengan tegangan referensi (Vc), resistansi internal sensor (Rs), dan resistansi beban (Rl).

```
VRl = [Vc / (Rs + Rl)] * RL
```

## Tahap 3: Tegangan Output pada Resistor Beban dalam Bentuk Alternatif
Persamaan alternatif dari Persamaan 2, menggabungkan tegangan referensi (Vc), resistansi internal sensor (Rs), dan resistansi beban (Rl) dalam bentuk lain.

```
VRl = [Vc * Rl / (Rs + Rl)]
```

## Tahap 4: Perhitungan Resistansi Internal Sensor Rs
Persamaan ini membantu menghitung resistansi internal sensor (Rs) berdasarkan tegangan referensi (Vc), tegangan output pada resistor beban (VRl), dan resistansi beban (Rl).

```
Rs = (Vc * Rl) / (VRl - Rl)
```

## Tahap 5: Hubungan Antara Resistansi Internal Sensor Rs dan Ratio Rs/R0
Persamaan ini menghubungkan resistansi internal sensor (Rs) dengan rasio antara resistansi internal sensor dalam udara segar (R0) dan Rs.

```
Rs / R0 = 3.6
```

## Tahap 6: Pembuatan Persamaan untuk Menghitung Concentration Gas
Persamaan ini membantu menghitung konsentrasi gas berdasarkan nilai-nilai log-log dari rasio dan konsentrasi yang diperoleh dari grafik log-log. Ini melibatkan perhitungan miring (slope) m dan konstanta intercept b.

### Bagian 6.1: Menentukan Data dari Kurva Grafik
Ambil data dari kurva grafik pada datasheet sensor. Dapatkan setidaknya dua titik pada kurva yang mewakili perubahan resistansi dan konsentrasi gas. 

Tentukan nilai x0 dan y0 sebagai nilai x dan y pada salah satu titik yang Anda pilih.

Kunjungi situs : https://plotdigitizer.com. Panduan ekstrak data dari gambar grafik datasheet sensor di: https://plotdigitizer.com/extrac-data-graph-image

### Bagian 6.2: Hitung Nilai Miring (Slope) dan Konstanta Intercept (b)
Hitung nilai miring (slope) m menggunakan persamaan:

```
m = log(y / y0) / log(x / x0)
```

Hitung nilai konstanta intercept b menggunakan persamaan:

```
b = log(0.9) - (-0.318) * log(5000)
```

### Bagian 6.3: Menghitung Konsentrasi Gas
Setelah mendapatkan nilai m dan b, hitung nilai log(x) dengan persamaan:

```
log(x) = [log(y) - b] / m
```

Terakhir, hitung nilai konsentrasi gas x dengan persamaan:

```
x = 10^(log(y) - b) / m
```

Dalam praktiknya, Anda perlu memiliki nilai rasio y/y0 yang diukur dari sensor Anda. Setelah mendapatkan nilai ini, Anda bisa menggunakannya untuk menghitung nilai x, yang akan menjadi estimasi konsentrasi gas dalam satuan PPM.
