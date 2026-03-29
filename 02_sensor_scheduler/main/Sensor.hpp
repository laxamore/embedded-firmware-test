#pragma once

#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_log.h"

class Sensor {
public:
    /**
     * @brief Construct a new Sensor object
     * 
     * @param name Name of the sensor
     * @param busMutex Shared mutex for bus arbitration
     * @param startDelayMs Initial delay to stagger bus access
     */
    Sensor(const std::string& name, SemaphoreHandle_t busMutex, uint32_t startDelayMs);
    ~Sensor();

    void start();

private:
    std::string _name;
    SemaphoreHandle_t _busMutex;
    uint32_t _startDelayMs;
    TaskHandle_t _taskHandle;

    static void sensorTask(void* pvParameters);
};
