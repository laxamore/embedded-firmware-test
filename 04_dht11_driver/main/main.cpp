#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "dht11.hpp"
#include "esp_log.h"

static const char* TAG = "MAIN_APP";
static const gpio_num_t DHT11_GPIO = GPIO_NUM_4;

extern "C" void app_main() {
    DHT11 sensor(DHT11_GPIO);
    
    printf("DHT11 Sensor Driver Initialized (GPIO %d)\n", DHT11_GPIO);

    while (true) {
        float humidity, temperature;
        esp_err_t ret = sensor.read(&humidity, &temperature);
        
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Humidity: %.1f %%, Temperature: %.1f °C", humidity, temperature);
        } else {
            ESP_LOGE(TAG, "Failed to read sensor (Error: %d)", ret);
        }

        // DHT11 requires at least 2 seconds between readings
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
