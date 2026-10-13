#include <Arduino.h>
#include <WiFi.h>

void networkTask(void *pvParameters);
void testFlashMemory();

// I2S Pin Configuration for DAC PCM5102A in ESP32-S3
#define I2S_DOUT 11 // DIN to PCM5102A
#define I2S_BCLK 12 // BCLK
#define I2S_LRC 13  // LRC / WSEL

void setup()
{
    Serial.begin(115200);

    // Check PSRAM availability
    if (psramInit())
    {
        Serial.printf("PSRAM Total: %d bytes\n", ESP.getPsramSize());
        Serial.printf("PSRAM Free: %d bytes\n", ESP.getFreePsram());
    }
    else
    {
        Serial.println("Failed to initialize PSRAM!");
    }

    testFlashMemory();

    // // Initiate Audio I2S to DAC PCM5102A
    // audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    // audio.setVolume(15); // Volume 0 - 21

    // Run Network Task in Core 0
    xTaskCreatePinnedToCore(
        networkTask,   // Task Function
        "NetworkTask", // Task Name
        8192,          // Stack Size
        NULL,          // Parameter
        1,             // Priority
        NULL,          // Handle
        0              // Pinned to Core 0
    );
}

void loop()
{
    // put your main code here, to run repeatedly:
}

void networkTask(void *pvParameters)
{
    // This place to setup WiFiManager / cSpot Spotify Connect
    Serial.println("Core 0: Network Task Running...");

    // Wifi Dummy Example
    WiFi.mode(WIFI_STA);
    WiFi.begin("JAYA MANDIRI", "mirzasaswan");
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi Connected!");
    Serial.println("Local IP: " + WiFi.localIP().toString());

    vTaskDelete(NULL);
}

void testFlashMemory() {
  Serial.println("--- Flash Memory Test ---");
  
  // Get Flash memory size
  size_t flashSize = ESP.getFlashChipSize();
  Serial.printf("Flash Size: %d bytes (%.2f MB)\n", flashSize, flashSize / 1024.0 / 1024.0);
  
  // Get Flash chip speed
  uint32_t flashSpeed = ESP.getFlashChipSpeed();
  Serial.printf("Flash Speed: %d MHz\n", flashSpeed / 1000000);
  
  // Get Flash chip mode
  uint8_t flashMode = ESP.getFlashChipMode();
  Serial.printf("Flash Mode: %d\n", flashMode);
  
  Serial.println("✅ Flash memory information retrieved!");
  Serial.println();
}