# IoT-Based Oil Storage Tank Monitoring System

## Project Overview

This project aims to develop an **IoT-based Oil Storage Tank Monitoring System** using an **ESP32 microcontroller**, **DHT11** sensor for temperature measurements, and an **ultrasonic sensor** for monitoring oil levels. The system communicates with the cloud platform **ThingSpeak** to store and visualize the data for remote monitoring.

### Features:
- **Real-time Temperature Monitoring**: The DHT11 sensor is used to measure the temperature inside the oil storage tank.
- **Oil Level Detection**: An ultrasonic sensor is used to measure the oil level by detecting the distance from the sensor to the oil surface.
- **Cloud Integration**: Data is sent to **ThingSpeak** for visualization, where temperature and oil level can be monitored remotely.

## Hardware Components

- **ESP32**: Microcontroller with Wi-Fi capabilities to send data to ThingSpeak.
- **DHT11**: Temperature sensor used to measure the oil tank temperature.
- **HC-SR04**: Ultrasonic sensor for measuring the distance (oil level).
- **ThingSpeak**: Cloud platform used to store and visualize the sensor data.

## Software Requirements

- **Arduino IDE**: For coding and uploading to the ESP32.
- **ThingSpeak**: Cloud platform for real-time data visualization and monitoring.

## Setup Instructions

1. **Install Required Libraries**:
   - **DHT** library for the DHT11 sensor.
   - **NewPing** library for the ultrasonic sensor.
   - **ThingSpeak** library for cloud communication.

2. **Configure Wi-Fi**:
   Replace the placeholders in the code with your Wi-Fi credentials:
   ```cpp
   #define WIFI_SSID "your_wifi_ssid"
   #define WIFI_PASSWORD "your_wifi_password"
