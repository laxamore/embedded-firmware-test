#include <stdio.h>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "LED.hpp"

extern "C" void app_main() {
    LED red("Red", LED::ActiveLevel::LOW, 10.0f);
    LED green("Green", LED::ActiveLevel::HIGH, 5.0f);
    LED blue("Blue", LED::ActiveLevel::HIGH, 2.0f);

    printf("Starting Manual Test Sequence...\n");
    
    red.on();
    vTaskDelay(pdMS_TO_TICKS(500));
    red.off();
    vTaskDelay(pdMS_TO_TICKS(500));

    printf("Starting LED Blink Simulation...\n");

    red.blink();
    green.blink();
    blue.blink();

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
