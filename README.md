# Embedded Firmware Test Repository

This repository contains a collection of four firmware implementation challenges for the ESP-IDF framework (C/C++), focusing on Object-Oriented Programming (OOP), FreeRTOS concurrency, shared resource arbitration, and low-level sensor driver development.

## Projects Overview

### [01_led_control](./01_led_control)
**Objective**: Independent control of three LEDs (Red, Green, Blue) using OOP.
- **Key Features**: Encapsulated `LED` class with private FreeRTOS blink tasks, handling Active HIGH/LOW logic and independent frequencies.
- **Verification**: `pytest` suite for timing and logic validation in QEMU.

### [02_sensor_scheduler](./02_sensor_scheduler)
**Objective**: Periodic scheduling for 5 sensors sharing a single data bus.
- **Problem**: 5 sensors, each requiring 40ms bus time, must report every 200ms with <5ms tolerance.
- **Solution**: 40ms staggered task initialization and Mutex-based bus arbitration to achieve 100% bus utilization efficiency.

### [03_concurrent_system](./03_concurrent_system)
**Objective**: Producer-Consumer pattern for a sensor-to-server pipeline.
- **Design**: A "Sensor Task" updates a shared buffer with 5 readings every 500ms, while a "Communication Task" retrieves and transmits that data every 1000ms.
- **Safety**: Thread-safe access via ESP-IDF Mutexes to ensure data integrity during concurrent operations.

### [04_dht11_driver](./04_dht11_driver)
**Objective**: Low-level 1-wire protocol driver for the DHT11 Humidity/Temperature sensor.
- **Implementation**: Precise microsecond bit-banging based on the official datasheet.
- **Features**: Start pulse signaling, handshake response detection, 40-bit data acquisition, and CRC checksum verification.

---

## Environment & Testing

All projects are developed for **ESP-IDF v5.0+** and targets **ESP32-C3** or **ESP32-C6**.

### Automated Simulation
Each project includes a `pytest` suite (where applicable) configured to run on the real **Espressif QEMU** emulator, allowing verification of firmware logic without physical hardware.

#### To run simulation:
1. Ensure ESP-IDF is exported.
2. Ensure QEMU is installed and exists in PATH.
3. Run `pytest` within the project directory.

```bash
# Example
cd 01_led_control
export PATH="$(pwd)/bin:$PATH"
pytest --target esp32c3 pytest_main.py
```
