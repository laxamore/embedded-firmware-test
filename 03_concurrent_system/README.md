# 03-Concurrent System (Mutex Protected Buffer)

This project demonstrates a thread-safe producer-consumer pattern using two concurrent tasks and a shared data buffer.

## Architecture

- **Shared Buffer**: A `struct` containing 5 sensor readings and an update counter.
- **Mutex Protection**: Access to the shared buffer is synchronized using an ESP-IDF Mutex (`SemaphoreHandle_t`).
- **Sensor Task (Producer)**:
  - Generates mock sensor data.
  - Updates the buffer every **500 ms**.
- **Communication Task (Consumer)**:
  - Reads the latest buffer state.
  - Simulates server transmission every **1000 ms**.

## Project Structure

- `main/main.cpp`: Core logic containing both task definitions and the shared buffer.
- `main/CMakeLists.txt`: Build configuration.
- `pytest_main.py`: Concurrency and data integrity test suite.

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

The `pytest` suite verifies that the Communication Task receives consistent data that matches the Sensor Task's update count.
Example Log:
```text
I (1119) CONCURRENT_SYS: Sensor Task: Updating buffer (Cycle 2)...
I (1119) CONCURRENT_SYS: Communication Task: Sending data (Buffer Update Count: 2)
Data Payload: [S1:2.0, S2:3.1, S3:4.2, S4:5.3, S5:6.4]
```
The data `S1:2.0` correctly corresponds to `Cycle 2`.