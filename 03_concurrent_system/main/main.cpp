#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_log.h"

static const char* TAG = "CONCURRENT_SYS";

// Shared data structure
struct SharedDataBuffer {
    float sensors[5];
    uint32_t updateCount;
};

// Global shared resources
static SharedDataBuffer g_sharedBuffer;
static SemaphoreHandle_t g_bufferMutex;

/**
 * @brief Sensor Task: Reads data from 5 sensors and updates the shared buffer.
 */
void sensor_task(void* pvParameters) {
    uint32_t counter = 0;
    while (true) {
        // Simulate reading 5 sensors
        float mockData[5];
        for (int i = 0; i < 5; ++i) {
            mockData[i] = (float)(counter + i * 1.1);
        }

        // Access shared buffer safely
        if (xSemaphoreTake(g_bufferMutex, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "Sensor Task: Updating buffer (Cycle %lu)...", (unsigned long)counter);
            memcpy(g_sharedBuffer.sensors, mockData, sizeof(mockData));
            g_sharedBuffer.updateCount = counter;
            xSemaphoreGive(g_bufferMutex);
        }

        counter++;
        vTaskDelay(pdMS_TO_TICKS(500)); // Update every 500ms
    }
}

/**
 * @brief Communication Task: Periodically retrieves data and sends it to "server".
 */
void communication_task(void* pvParameters) {
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000)); // Send every 1000ms

        // Access shared buffer safely
        if (xSemaphoreTake(g_bufferMutex, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "Communication Task: Sending data (Buffer Update Count: %lu)", 
                     (unsigned long)g_sharedBuffer.updateCount);
            
            // Log sensor values
            printf("Data Payload: [S1:%.1f, S2:%.1f, S3:%.1f, S4:%.1f, S5:%.1f]\n",
                   g_sharedBuffer.sensors[0], g_sharedBuffer.sensors[1],
                   g_sharedBuffer.sensors[2], g_sharedBuffer.sensors[3],
                   g_sharedBuffer.sensors[4]);
            
            xSemaphoreGive(g_bufferMutex);
        }
    }
}

extern "C" void app_main() {
    // Initialize Mutex
    g_bufferMutex = xSemaphoreCreateMutex();
    memset(&g_sharedBuffer, 0, sizeof(g_sharedBuffer));

    printf("Concurrent System Simulation Started\n");

    // Create Tasks
    xTaskCreate(sensor_task, "sensor_task", 4096, NULL, 5, NULL);
    xTaskCreate(communication_task, "comm_task", 4096, NULL, 4, NULL);

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
