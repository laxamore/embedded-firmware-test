#pragma once

#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

enum class LEDMode {
    OFF,
    ON,
    BLINK
};

class LED {
public:
    enum class ActiveLevel {
        LOW = 0,
        HIGH = 1
    };

    /**
     * @brief Construct a new LED object
     * 
     * @param name Name of the LED (for logging)
     * @param activeLevel Active level (LOW or HIGH)
     * @param frequency Frequency in Hz for blink mode
     */
    LED(const std::string& name, ActiveLevel activeLevel, float frequency);
    ~LED();

    void on();
    void off();
    void blink();

private:
    std::string _name;
    ActiveLevel _activeLevel;
    float _frequency;
    LEDMode _mode;
    TaskHandle_t _blinkTaskHandle;

    static void blinkTask(void* pvParameters);
    void updateState(bool state);
    void stopBlinkTask();
};
