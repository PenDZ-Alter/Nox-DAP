#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// Objek WebServer standar untuk halaman kustom Anda setelah terhubung
WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>Halo dari ESP32-S3!</h1><p>Wi-Fi berhasil terhubung dan Captive Portal sukses.</p>");
}

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);
  delay(1000);

  // Inisialisasi WiFiManager
  WiFiManager wm;

  // Jika ingin mereset pengaturan WiFi yang tersimpan sebelumnya untuk pengujian, aktifkan baris di bawah ini sekali:
  wm.resetSettings();

  // Memulai portal konfigurasi otomatis
  // Jika gagal terhubung ke WiFi sebelumnya, ESP32 akan membuat AP dengan nama "ESP32-S3-Config"
  Serial.println("Memulai WiFiManager...");
  bool res = wm.autoConnect("ESP32-S3-Config", "123456789"); // Tambahkan password jika perlu, misal: wm.autoConnect("ESP32-S3-Config", "password123");

  if (!res) {
    Serial.println("Gagal terhubung atau timeout konfigurasi.");
    // ESP32 akan melakukan restart atau masuk mode deep sleep sesuai kebutuhan
    ESP.restart();
    delay(1000);
  } else {
    // Berhasil terhubung ke Wi-Fi
    Serial.println("Berhasil terhubung ke jaringan Wi-Fi!");
    Serial.print("Alamat IP ESP32-S3: ");
    Serial.println(WiFi.localIP());
  }

  // Menyiapkan rute Web Server setelah terhubung
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server aktif.");
}

void loop() {
  // Menangani klien web yang mengakses server
  server.handleClient();
}