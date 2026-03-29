#include "Sensor.hpp"

static const char* TAG = "SENSOR_SYSTEM";

Sensor::Sensor(const std::string& name, SemaphoreHandle_t busMutex, uint32_t startDelayMs)
    : _name(name), _busMutex(busMutex), _startDelayMs(startDelayMs), _taskHandle(nullptr) {
}

Sensor::~Sensor() {
    if (_taskHandle != nullptr) {
        vTaskDelete(_taskHandle);
    }
}

void Sensor::start() {
    xTaskCreate(sensorTask, (_name + "_task").c_str(), 4096, this, 5, &_taskHandle);
}

void Sensor::sensorTask(void* pvParameters) {
    Sensor* sensor = static_cast<Sensor*>(pvParameters);
    TickType_t lastWakeTime;
    const TickType_t period = pdMS_TO_TICKS(200);
    const TickType_t readDuration = pdMS_TO_TICKS(40);
    const TickType_t sendDuration = pdMS_TO_TICKS(40);

    // Initial stagger delay
    vTaskDelay(pdMS_TO_TICKS(sensor->_startDelayMs));
    
    lastWakeTime = xTaskGetTickCount();

    while (true) {
        // vTaskDelayUntil ensures fixed-rate execution even if processing takes time
        vTaskDelayUntil(&lastWakeTime, period);

        // 1. Acquire Bus Mutex for Reading
        if (xSemaphoreTake(sensor->_busMutex, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "[%s] Reading from Bus (Start: %lu ms)", 
                     sensor->_name.c_str(), 
                     (unsigned long)(xTaskGetTickCount() * portTICK_PERIOD_MS));
            
            // Simulate 40ms bus read
            vTaskDelay(readDuration);
            
            xSemaphoreGive(sensor->_busMutex);
            ESP_LOGI(TAG, "[%s] Bus Released", sensor->_name.c_str());
        }

        // 2. Transmit Data (Simulate 40ms send)
        ESP_LOGI(TAG, "[%s] Sending Data...", sensor->_name.c_str());
        vTaskDelay(sendDuration);
        
        ESP_LOGI(TAG, "[%s] Transmission Complete (End: %lu ms)", 
                 sensor->_name.c_str(),
                 (unsigned long)(xTaskGetTickCount() * portTICK_PERIOD_MS));
    }
}
