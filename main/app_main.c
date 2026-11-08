/*
 * HomeGrid FW-01: board-neutral ESP-IDF diagnostic application.
 *
 * Scope: identify the running SoC and report basic runtime information.
 * This application intentionally does not configure GPIO, operate relays,
 * initialise Wi-Fi/BLE, connect to a network, or touch mains-connected gear.
 */
#include <inttypes.h>
#include <stdint.h>

#include "esp_chip_info.h"
#include "esp_idf_version.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "homegrid_fw01";

void app_main(void)
{
    esp_chip_info_t chip_info = {0};
    esp_chip_info(&chip_info);

    ESP_LOGI(TAG, "HomeGrid FW-01 diagnostic firmware starting");
    ESP_LOGI(TAG, "ESP-IDF version: %s", esp_get_idf_version());
    ESP_LOGI(TAG, "Chip model enum: %u", (unsigned int)chip_info.model);
    ESP_LOGI(TAG, "Chip revision: %u", (unsigned int)chip_info.revision);
    ESP_LOGI(TAG, "CPU cores: %u", (unsigned int)chip_info.cores);
    ESP_LOGI(TAG, "Chip feature flags: 0x%08" PRIx32,
             (uint32_t)chip_info.features);
    ESP_LOGI(TAG, "2.4 GHz Wi-Fi capability: %s",
             (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "yes" : "no");
    ESP_LOGI(TAG, "BLE capability: %s",
             (chip_info.features & CHIP_FEATURE_BLE) ? "yes" : "no");
    ESP_LOGI(TAG, "Initial free heap: %" PRIu32 " bytes",
             (uint32_t)esp_get_free_heap_size());

    /* app_main is already executed as a FreeRTOS task. */
    for (;;) {
        ESP_LOGI(TAG,
                 "Heartbeat; free_heap=%" PRIu32
                 " bytes; minimum_free_heap=%" PRIu32 " bytes",
                 (uint32_t)esp_get_free_heap_size(),
                 (uint32_t)esp_get_minimum_free_heap_size());
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
