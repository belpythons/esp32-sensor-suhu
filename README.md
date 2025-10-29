# ESP32: Web Server Monitoring Suhu & Kelembaban

Ini adalah proyek IoT sederhana yang menggunakan ESP32 dan sensor DHT22 untuk memantau suhu serta kelembaban. Data sensor disajikan melalui halaman web lokal yang di-hosting langsung oleh ESP32.

Proyek ini sepenuhnya dapat disimulasikan menggunakan Wokwi.

[![Simulasikan di Wokwi](https://wokwi.com/static/beta.svg)](https://wokwi.com/projects/446151842311108609)

## 🚀 Fitur

* Membaca data suhu (dalam Celcius) dan kelembaban (dalam %).
* Terhubung ke jaringan WiFi (fisik atau virtual di Wokwi).
* Membuat server web sederhana pada port 80.
* Menampilkan data sensor secara *real-time* (data diperbarui setiap kali halaman web di-*refresh*).

## 🛠️ Kebutuhan Hardware

* **Microcontroller:** ESP32 (Board ESP32 DevKit)
* **Sensor:** DHT22 (Sensor Suhu dan Kelembaban)
* **Kabel Jumper**

## 🖥️ Kebutuhan Software

* [Wokwi Simulator](https://wokwi.com/) (untuk simulasi)
* [Arduino IDE](https://www.arduino.cc/en/software) (untuk *upload* ke hardware fisik)
* **Library Arduino:**
    * `WiFi.h` (terintegrasi dengan ESP32)
    * `DHT sensor library` oleh Adafruit

## 🔌 Skema Rangkaian (Wiring)

| Komponen | Pin | ESP32 |
| :--- | :--- | :--- |
| **DHT22** | `VCC` | `3V3` |
| | `GND` | `GND` |
| | `SDA/Data`| `GPIO 4` |

*(Anda bisa melihat skema visualnya langsung di [Wokwi](https://wokwi.com/projects/446151842311108609))*

## 🔧 Cara Menjalankan

Ada dua cara untuk menjalankan proyek ini: melalui simulasi Wokwi atau menggunakan hardware fisik.

### 1. Simulasi di Wokwi (Cara Cepat)

1.  Buka link proyek Wokwi: **[https://wokwi.com/projects/446151842311108609](https://wokwi.com/projects/446151842311108609)**
2.  Pastikan kode di `sketch.ino` menggunakan kredensial WiFi untuk Wokwi:
    ```cpp
    const char* ssid = "WOKWI-GUEST";
    const char* password = "";
    ```
3.  Tekan tombol **"Start"** (▶️) hijau untuk memulai simulasi.
4.  Tunggu hingga di **Serial Monitor** muncul pesan "WiFi terhubung." dan sebuah alamat IP (contoh: `10.10.0.2`).
5.  Sebuah tab baru **"Web Server"** akan muncul di atas area simulasi. Klik tab tersebut.
6.  Anda akan melihat halaman web yang menampilkan data suhu dan kelembaban.

### 2. Menggunakan Hardware Fisik

1.  **Siapkan Hardware:** Rakit komponen sesuai dengan [Skema Rangkaian](#-skema-rangkaian-wiring) di atas.
2.  **Buka Arduino IDE:** Salin kode dari file `sketch.ino` di Wokwi ke Arduino IDE Anda.
3.  **Install Library:** Buka `Tools > Manage Libraries...` dan install "DHT sensor library by Adafruit".
4.  **Ubah Kredensial WiFi:** Ganti `ssid` dan `password` agar sesuai dengan jaringan WiFi lokal Anda.
    ```cpp
    const char* ssid = "Nama_WiFi_Anda";
    const char* password = "Password_WiFi_Anda";
    ```
5.  **Upload Kode:**
    * Pilih board Anda (`Tools > Board > ESP32 Arduino > ESP32 Dev Module`).
    * Pilih Port COM yang benar.
    * Klik tombol "Upload".
6.  **Akses Halaman Web:**
    * Buka **Serial Monitor** di Arduino IDE (atur baud rate ke `115200`).
    * Tunggu hingga ESP32 terhubung ke WiFi dan menampilkan alamat IP-nya.
    * Buka alamat IP tersebut di browser (Chrome, Firefox, dll.) pada perangkat (HP atau komputer) yang terhubung ke jaringan WiFi yang sama.

## 📄 Kode Sumber

Kode utama dapat ditemukan di file `sketch.ino`. Kode ini melakukan inisialisasi sensor DHT, terhubung ke WiFi, dan menjalankan loop server untuk menangani permintaan web yang masuk.

```cpp
/*
 * Salinan singkat dari kode utama
 * (Versi lengkap ada di file sketch.ino)
 */

#include <WiFi.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Ganti ini untuk hardware fisik
// Gunakan "WOKWI-GUEST" dan "" untuk simulasi Wokwi
const char* ssid = "WOKWI-GUEST";
const char* password = "";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }

  Serial.println("WiFi terhubung.");
  server.begin();
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.println("<!DOCTYPE html><html>");
  client.println("<h1>Monitoring Suhu & Kelembaban</h1>");
  client.println("<p>Suhu: " + String(suhu) + " *C</p>");
  client.println("<p>Kelembaban: " + String(kelembaban) + " %</p>");
  client.println("</html>");
}