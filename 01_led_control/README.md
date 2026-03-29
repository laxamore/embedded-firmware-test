# 01-LED Control (ESP-IDF OOP C++)

This project implements an independent LED control system for three LEDs (Red, Green, Blue) using Object-Oriented Programming (OOP) in C++ with the ESP-IDF framework. It features concurrent blinking using FreeRTOS tasks and a comprehensive simulation suite.

## Requirements

The system manages three LEDs with independent frequencies and specific active levels:

| Name  | Active Level | Mode  | Blink Frequency |
| :---- | :----------- | :---- | :-------------- |
| Red   | **LOW**      | Blink | 10 Hz           |
| Green | **HIGH**     | Blink | 5 Hz            |
| Blue  | **HIGH**     | Blink | 2 Hz            |

## Project Structure

- `main/LED.hpp` & `main/LED.cpp`: Reusable `LED` class with encapsulated state and private FreeRTOS tasks.
- `main/main.cpp`: Application entry that instantiates the Red, Green, and Blue LEDs.
- `pytest_main.py`: Pytest suite for automated log verification in simulation or on hardware.
- `pytest.ini`: Pytest configuration for clean logs and registered markers.
- `bin/`: Contains compatibility shims for `esptool` (to support `merge-bin` syntax in older environments).

## Prerequisites

- **ESP-IDF v5.0+**: Ensure the environment is exported (`. export.sh`).
- **QEMU**: Espressif's QEMU binaries for RISC-V/Xtensa.

## How to Build

1. Set the target (e.g., `esp32c3` for simulation or `esp32c6` for hardware):
   ```bash
   idf.py set-target esp32c3
   ```

2. Build the project:
   ```bash
   idf.py build
   ```

## How to Test (Simulation)

The project is configured to run on the real Espressif QEMU emulator.

1. **Set up the environment PATH**:
   Add the local `bin/` shim:
   ```bash
   export PATH="$(pwd)/bin:$PATH"
   ```

2. **Run Pytest**:
   ```bash
   pytest --target esp32c3 pytest_main.py
   ```
   *Note: Logs will be visible in the console because `-s` is configured as a default in `pytest.ini`.*

## How to Flash (Hardware)

Connect your ESP32 device and run:
```bash
idf.py flash monitor
```

## Simulation Output Example

The application uses `ESP_LOGI` to report state changes. In simulation, you will see output like this:
```text
I (118) LED_CONTROLLER: LED Red initialized (Active: LOW, Freq: 10.0 Hz)
I (118) LED_CONTROLLER: LED Green initialized (Active: HIGH, Freq: 5.0 Hz)
Starting LED Blink Simulation...
I (118) LED_CONTROLLER: [Red] Logical: ON | Physical: 0
I (118) LED_CONTROLLER: [Green] Logical: ON | Physical: 1
```
- **Logical**: `ON` or `OFF` representation.
- **Physical**: `1` (High) or `0` (Low), correctly mapped based on the LED's **Active Level**.