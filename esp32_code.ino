#include <WiFiManager.h>
#include <WiFiClient.h>

// ThingSpeak server and credentials
const char *server = "api.thingspeak.com";
const char *apiKey = "YOUR_THINGSPEAK_WRITE_API_KEY"; // Replace with your ThingSpeak Write API Key

// Pin definitions
#define FLOW_SENSOR_PIN 5 // Flow sensor pin (YF-S201)
#define MOISTURE_SENSOR_PIN 4 // Moisture sensor pin

// Variables
WiFiClient client;
volatile int flowCount = 0;
int tankFillCount = 0;
int moistureLevel = 0;
unsigned long previousMillis = 0;
const long interval = 15000; // ThingSpeak requires at least a 15-second delay

// Interrupt function for the flow sensor
void IRAM_ATTR flowSensorInterrupt() {
  flowCount++;
}

void setup() {
  Serial.begin(115200);

  // Setup WiFiManager
  WiFiManager wifiManager;
  wifiManager.autoConnect("ThingSpeak_AP"); // Captive portal for WiFi credentials
  Serial.println("Connected to WiFi");

  // Initialize sensors
  pinMode(FLOW_SENSOR_PIN, INPUT);
  pinMode(MOISTURE_SENSOR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), flowSensorInterrupt, FALLING);
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to send data
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Calculate water usage
    float flowRate = flowCount / 7.5; // Calibration factor for YF-S201 flow sensor
    flowCount = 0; // Reset flow count after calculation

    // Read moisture sensor (for tank fills)
    moistureLevel = analogRead(MOISTURE_SENSOR_PIN);
    if (moistureLevel < 500) { // Assuming below 500 means tank is filled
      tankFillCount++;
    }

    // Debugging data
    Serial.print("Flow Rate: ");
    Serial.print(flowRate);
    Serial.print(" L/min, Tank Fills: ");
    Serial.println(tankFillCount);

    // Prepare HTTP POST data
    String postStr = "api_key=";
    postStr += apiKey;
    postStr += "&field1=" + String(flowRate);     // Field 1: Water usage
    postStr += "&field2=" + String(tankFillCount); // Field 2: Tank fills

    // Send data to ThingSpeak
    if (client.connect(server, 80)) {
      Serial.println("Connected to ThingSpeak");
      client.println("POST /update HTTP/1.1");
      client.println("Host: api.thingspeak.com");
      client.println("Connection: close");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(postStr.length());
      client.println();
      client.print(postStr);
      Serial.println("Data sent to ThingSpeak");
    } else {
      Serial.println("Failed to connect to ThingSpeak");
    }
    client.stop(); // Close the connection
  }
}
