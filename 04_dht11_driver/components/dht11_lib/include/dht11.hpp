#pragma once

#include "driver/gpio.h"
#include "esp_err.h"

class DHT11 {
public:
    /**
     * @brief Initialize the DHT11 sensor on a specific GPIO.
     * 
     * @param gpio GPIO number connected to the DHT11 data pin.
     */
    DHT11(gpio_num_t gpio);

    /**
     * @brief Read data from the DHT11 sensor.
     * 
     * @param humidity Pointer to store humidity percentage.
     * @param temperature Pointer to store temperature in Celsius.
     * @return esp_err_t ESP_OK on success, ESP_ERR_TIMEOUT or ESP_ERR_INVALID_CRC on failure.
     */
    esp_err_t read(float* humidity, float* temperature);

private:
    gpio_num_t _gpio;

    /**
     * @brief Wait for the GPIO to reach a certain level within a timeout.
     * 
     * @param level Target level (0 or 1).
     * @param timeout_us Timeout in microseconds.
     * @return int Time waited in microseconds, or -1 on timeout.
     */
    int wait_for_level(uint32_t level, int timeout_us);

    /**
     * @brief Internal method for the actual bit-banging protocol.
     */
    esp_err_t read_dht11(float* humidity, float* temperature);
};
