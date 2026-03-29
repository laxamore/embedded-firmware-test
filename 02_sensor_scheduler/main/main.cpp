#include <stdio.h>
#include <vector>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "Sensor.hpp"

extern "C" void app_main() {
    // Shared bus mutex
    SemaphoreHandle_t busMutex = xSemaphoreCreateMutex();
    
    // Create 5 sensors with staggered start times (40ms steps)
    // S1: 0ms, S2: 40ms, S3: 80ms, S4: 120ms, S5: 160ms
    // This perfectly fills the 200ms period with 40ms read slots.
    static std::vector<Sensor*> sensors;
    for (int i = 0; i < 5; ++i) {
        std::string name = "Sensor_" + std::to_string(i + 1);
        Sensor* s = new Sensor(name, busMutex, i * 40);
        sensors.push_back(s);
        s->start();
    }

    printf("Multi-Sensor Scheduler Started (Period: 200ms, Slots: 40ms)\n");

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
