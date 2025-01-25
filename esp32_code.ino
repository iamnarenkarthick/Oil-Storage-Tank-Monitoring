#include <WiFi.h>
#include <ThingSpeak.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <NewPing.h>

// Pin definitions
#define ONE_WIRE_BUS 4     // DS18B20 data pin
#define TRIGGER_PIN 12     // Trigger pin for ultrasonic sensor
#define ECHO_PIN 13        // Echo pin for ultrasonic sensor
#define MAX_DISTANCE 200   // Max distance to measure (in cm)

// WiFi credentials and ThingSpeak API key
#define WIFI_SSID "your_wifi_ssid"          // Replace with your WiFi SSID
#define WIFI_PASSWORD "your_wifi_password"  // Replace with your WiFi password
#define THINGSPEAK_API_KEY "UA529KDMFZGRN8F6"  // Replace with your ThingSpeak Write API Key

// Initialize OneWire and DallasTemperature libraries
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Initialize ultrasonic sensor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Initialize WiFi and ThingSpeak
WiFiClient client;

void setup() {
  Serial.begin(115200);
  sensors.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait for WiFi to connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  ThingSpeak.begin(client);  // Initialize ThingSpeak
  Serial.println("Connected to WiFi");
}

void loop() {
  // Request temperature data from DS18B20 sensor
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  // Get oil level data (distance measurement) from ultrasonic sensor
  float distance = sonar.ping_cm();

  if (temperature == DEVICE_DISCONNECTED_C || distance == 0) {
    Serial.println("Failed to read from sensors");
  } else {
    // Print values to Serial Monitor for debugging
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, ");
    Serial.print("Oil Level (Distance): ");
    Serial.print(distance);
    Serial.println(" cm");

    // Update ThingSpeak with sensor data
    ThingSpeak.setField(1, temperature);  // Field 1 for Temperature
    ThingSpeak.setField(2, distance);     // Field 2 for Oil Level (Distance)

    // Write to ThingSpeak every 15 seconds
    int responseCode = ThingSpeak.writeFields(THINGSPEAK_API_KEY);

    if (responseCode == 200) {
      Serial.println("Data successfully sent to ThingSpeak.");
    } else {
      Serial.print("Failed to send data. Error code: ");
      Serial.println(responseCode);
    }

    delay(15000);  // 15 seconds delay before sending next data
  }
}
