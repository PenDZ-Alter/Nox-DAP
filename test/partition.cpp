#include <Arduino.h>
#include "esp_partition.h"

void printPartitions()
{
    esp_partition_iterator_t it =
        esp_partition_find(
            ESP_PARTITION_TYPE_ANY,
            ESP_PARTITION_SUBTYPE_ANY,
            nullptr
        );

    Serial.println("\n=== ESP32 Partition Table ===");

    while (it != nullptr) {
        const esp_partition_t *p = esp_partition_get(it);

        Serial.printf(
            "Name: %-16s | Type: %u | Subtype: %u"
            " | Offset: 0x%06X | Size: 0x%06X (%u bytes)\n",
            p->label,
            (unsigned)p->type,
            (unsigned)p->subtype,
            (unsigned)p->address,
            (unsigned)p->size,
            (unsigned)p->size
        );

        it = esp_partition_next(it);
    }

    Serial.println("=============================");
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    printPartitions();
}

void loop()
{
}