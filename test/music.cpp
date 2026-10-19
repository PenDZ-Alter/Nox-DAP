
#include <Arduino.h>
#include <LittleFS.h>
#include <Audio.h>

Audio audio;

// I2S -> GY-PCM5102A
#define I2S_BCLK 4
#define I2S_DOUT 5
#define I2S_LRCK 6

const char *MP3_PATH =
    "data/A Memorable Fancy.mp3";

void setup()
{
    Serial.begin(115200);
    delay(1500);

    Serial.println();
    Serial.println("=== ESP32-S3 MP3 TEST ===");

    // Jangan format otomatis kalau mount gagal
    if (!LittleFS.begin(true, "/littlefs", 10, "spiffs"))
    {
        Serial.println("LittleFS gagal mount!");
        if (!LittleFS.begin(true, "/littlefs", 10, "spiffs"))
        {
            Serial.println("LittleFS gagal mount! Mencoba format...");
            if (LittleFS.format())
            {
                Serial.println("Format berhasil, mounting ulang...");
                if (!LittleFS.begin(false, "/littlefs", 10, "spiffs"))
                {
                    Serial.println("Masih gagal mount setelah format!");
                    return;
                }
            }
            else
            {
                Serial.println("Format gagal!");
                return;
            }
        }
        Serial.println("LittleFS berhasil mount!");
    }
    else
    {
        Serial.println("LittleFS berhasil mount!");

        File root = LittleFS.open("/");
        File file = root.openNextFile();

        while (file)
        {
            Serial.printf("File: %s | Size: %u bytes\n",
                          file.name(),
                          (unsigned int)file.size());
            file = root.openNextFile();
        }
    }

    // Serial.println("[OK] LittleFS mounted");

    if (!LittleFS.exists(MP3_PATH))
    {
        Serial.printf("[ERROR] File tidak ditemukan: %s\n",
                      MP3_PATH);
        Serial.println("Periksa folder data/ dan uploadfs.");
        return;
    }

    File mp3 = LittleFS.open(MP3_PATH, "r");

    if (!mp3)
    {
        Serial.println("[ERROR] File MP3 gagal dibuka!");
        return;
    }

    Serial.printf("[OK] File ditemukan, ukuran: %u bytes\n",
                  (unsigned int)mp3.size());
    mp3.close();

    audio.setPinout(I2S_BCLK, I2S_LRCK, I2S_DOUT);
    audio.setVolume(10); // Rentang umum: 0-21

    Serial.println("[INFO] Memulai playback MP3...");

    if (!audio.connecttoFS(LittleFS, MP3_PATH))
    {
        Serial.println("[ERROR] Gagal memulai playback!");
    }
    else
    {
        Serial.println("[OK] Permintaan playback dikirim");
    }
}

void loop()
{
    audio.loop();
    delay(1);
}

void audio_info(const char *info)
{
    Serial.print("[AUDIO] Audio Info: ");
    Serial.println(info);
}
