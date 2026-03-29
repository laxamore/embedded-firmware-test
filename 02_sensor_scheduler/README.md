# 02-Sensor Scheduler (Periodic Bus Arbitration)

This project implements a periodic scheduler for 5 sensors sharing a single data bus. It ensures each sensor performs high-precision reading and transmission without violating timing constraints or shared resource exclusion.

## Specifications

- **Period**: 200 ms per sensor (Tolerance: < 5 ms).
- **Bus Reading**: 40 ms (Must be sequential/exclusive).
- **Transmission**: 40 ms (Simulated total cycle of 80ms).
- **Total Sensors**: 5.

## Architecture

- **`Sensor` Class**: Encapsulates a FreeRTOS task and synchronization logic.
- **Mutex Arbitration**: A shared Mutex ensures that only one sensor accesses the bus at any given time.
- **Staggered Start**: Sensors are initialized with a 40ms offset (S1: 0ms, S2: 40ms, ... S5: 160ms) to perfectly tile the bus utilization across the 200ms period.
- **High Precision**: Uses `vTaskDelayUntil` to maintain a steady 200ms rate even with system jitter or processing overhead.

## Project Structure

- `main/Sensor.hpp` & `Sensor.cpp`: Sensor task logic and synchronization.
- `main/main.cpp`: Orchestrates the shared Mutex and 5 sensor instances.
- `pytest_main.py`: Automated timing verification.
- `pytest.ini`: Preconfigured defaults for clean simulation output.

## How to Test (Simulation)

1. **Set up the PATH**:
   ```bash
   export PATH="$(pwd)/bin:$PATH"
   ```

2. **Run Pytest**:
   ```bash
   pytest --target esp32c3 pytest_main.py
   ```

## Verification Results

The system maintains a perfect **200ms average interval** for each sensor.
Example Log:
```text
I (1439) SENSOR_SYSTEM: [Sensor_4] Reading from Bus (Start: 1320 ms)
I (1479) SENSOR_SYSTEM: [Sensor_4] Bus Released
I (1479) SENSOR_SYSTEM: [Sensor_5] Reading from Bus (Start: 1360 ms)
```
Consecutive reads start exactly 40ms apart, proving zero bus idle time and zero contention.