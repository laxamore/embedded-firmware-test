#include "LED.hpp"
#include <iostream>
#include "esp_log.h"

static const char* TAG = "LED_CONTROLLER";

LED::LED(const std::string& name, ActiveLevel activeLevel, float frequency)
    : _name(name), _activeLevel(activeLevel), _frequency(frequency), _mode(LEDMode::OFF), _blinkTaskHandle(nullptr) {
    ESP_LOGI(TAG, "LED %s initialized (Active: %s, Freq: %.1f Hz)", 
             _name.c_str(), 
             (_activeLevel == ActiveLevel::HIGH ? "HIGH" : "LOW"), 
             _frequency);
}

LED::~LED() {
    stopBlinkTask();
}

void LED::on() {
    stopBlinkTask();
    _mode = LEDMode::ON;
    updateState(true);
}

void LED::off() {
    stopBlinkTask();
    _mode = LEDMode::OFF;
    updateState(false);
}

void LED::blink() {
    if (_mode == LEDMode::BLINK) return;
    
    _mode = LEDMode::BLINK;
    xTaskCreate(blinkTask, (_name + "_task").c_str(), 2048, this, 5, &_blinkTaskHandle);
}

void LED::updateState(bool state) {
    // state is logical ON/OFF
    // physicalState depends on activeLevel
    bool physicalState = (_activeLevel == ActiveLevel::HIGH) ? state : !state;
    
    // Simulation: Print to console
    ESP_LOGI(TAG, "[%s] Logical: %s | Physical: %d", 
             _name.c_str(), 
             (state ? "ON" : "OFF"), 
             physicalState);
}

void LED::blinkTask(void* pvParameters) {
    LED* led = static_cast<LED*>(pvParameters);
    bool state = false;
    TickType_t delayTicks = (1000 / (led->_frequency * 2)) / portTICK_PERIOD_MS;
    
    if (delayTicks == 0) delayTicks = 1; // Minimum delay

    while (true) {
        state = !state;
        led->updateState(state);
        vTaskDelay(delayTicks);
    }
}

void LED::stopBlinkTask() {
    if (_blinkTaskHandle != nullptr) {
        vTaskDelete(_blinkTaskHandle);
        _blinkTaskHandle = nullptr;
    }
}
