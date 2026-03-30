#include "dht11.hpp"
#include "rom/ets_sys.h"
#include "esp_timer.h"
#include "esp_log.h"

static const char* TAG = "DHT11_LIB";

DHT11::DHT11(gpio_num_t gpio) : _gpio(gpio) {
    gpio_reset_pin(_gpio);
}

esp_err_t DHT11::read(float* humidity, float* temperature) {
    return read_dht11(humidity, temperature);
}

esp_err_t DHT11::read_dht11(float* humidity, float* temperature) {
    uint8_t data[5] = {0, 0, 0, 0, 0};

    // 1. Host sends Start Signal
    gpio_set_direction(_gpio, GPIO_MODE_OUTPUT);
    gpio_set_level(_gpio, 0);
    ets_delay_us(18000); // 18ms Minimum

    gpio_set_level(_gpio, 1);
    ets_delay_us(40); // 40us for DHT11 to detect

    // 2. DHT11 sends Response Signal
    gpio_set_direction(_gpio, GPIO_MODE_INPUT);
    
    // Wait for sensor to pull low (80us)
    if (wait_for_level(0, 100) < 0) return ESP_ERR_TIMEOUT;
    // Wait for sensor to pull high (54us)
    if (wait_for_level(1, 100) < 0) return ESP_ERR_TIMEOUT;
    // Wait for data start (pull low again for 80us)
    if (wait_for_level(0, 100) < 0) return ESP_ERR_TIMEOUT;

    // 3. Read 40 bits of data
    for (int i = 0; i < 40; i++) {
        // Wait for bit start (HIGH pulse begins)
        if (wait_for_level(1, 100) < 0) return ESP_ERR_TIMEOUT;

        // Measure HIGH duration
        int duration = wait_for_level(0, 100);
        if (duration < 0) return ESP_ERR_TIMEOUT;

        // datasheet: 24-28us = '0', 70us = '1'
        if (duration > 40) {
            data[i / 8] |= (1 << (7 - (i % 8)));
        }
    }

    // 4. Checksum Verification
    if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        ESP_LOGE(TAG, "CRC Error: %02x %02x %02x %02x != %02x", data[0], data[1], data[2], data[3], data[4]);
        return ESP_ERR_INVALID_CRC;
    }

    *humidity = (float)data[0] + (float)data[1] / 10.0f;
    *temperature = (float)data[2] + (float)data[3] / 10.0f;

    return ESP_OK;
}

int DHT11::wait_for_level(uint32_t level, int timeout_us) {
    int count = 0;
    while (gpio_get_level(_gpio) != (int)level) {
        if (count >= timeout_us) return -1;
        count++;
        ets_delay_us(1);
    }
    return count;
}
