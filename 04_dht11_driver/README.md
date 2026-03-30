# 04-DHT11 Driver (ESP-IDF Component)

This project implements a reusable DHT11 sensor library for ESP-IDF. It was implemented by using bit-banging method.

## Features

- **Standard Library structure**: Located in `components/dht11_lib`.
- **Bit-banging implementation**: Follows the official DHT11 communication protocol.
- **Checksum Verification**: Validates 40 bits of data using the 8-bit checksum.

## Protocol Summary (from Datasheet)

1.  **Start Signal**: Host pulls LOW for 18ms.
2.  **Handshake**: Sensor pulls LOW for 54us, then HIGH for 80us.
3.  **Data Transmission**: 40 bits (5 bytes: Hum-int, Hum-dec, Temp-int, Temp-dec, Checksum).
4.  **Bit Timing**: 0 = 24-28us HIGH pulse, 1 = 70us HIGH pulse.

## Project Structure

- `components/dht11_lib/`: The driver logic.
- `main/main.cpp`: An application polling the sensor every 2 seconds.