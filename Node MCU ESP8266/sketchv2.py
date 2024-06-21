#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <SPI.h> // Not strictly necessary for MPU6050, but may be required on some boards

// WiFi credentials (replace with your network SSID and password)
const char* ssid = "your_ssid";
const char* password = "your_password";

// Static IP configuration (replace with desired IP, subnet mask, and gateway)
IPAddress local_IP(172, 21, 0, 18);
IPAddress subnet(255, 255, 248, 0);
IPAddress gateway(172, 21, 7, 254);

// MPU6050 connection
Adafruit_MPU6050 mpu;

// Variables to store sensor data
sensors_event_t a, g, temp; //a:acceleration(m/s^2) g:gyro(rad/sec) temp:temparature(celsius)

// Web server port
const int serverPort = 80;
WiFiServer server(serverPort);

void setup() {
  Serial.begin(115200);
  // Initialize I2C communication for MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(2000);
    }
  }
  Serial.println("MPU6050 Found!");

  // Configure WiFi with static IP
  WiFi.config(local_IP, gateway, subnet);
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  // Start web server
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  WiFiClient client = server.available();
  mpu.getEvent(&a, &g, &temp);

  // Print sensor data to serial monitor for debugging
  Serial.print(a.acceleration.x);
  Serial.print("\t");
  Serial.print(a.acceleration.y);
  Serial.print("\t");
  Serial.print(a.acceleration.z);
  Serial.print("\t");
  Serial.print(g.gyro.x);
  Serial.print("\t");
  Serial.print(g.gyro.y);
  Serial.print("\t");
  Serial.print(g.gyro.z);
  Serial.print("\t");
  Serial.print(temp.temperature);
  Serial.println();
  delay(1000);

  if (client) {
    // Read sensor data (already done in the beginning of loop)

    // Prepare web page content
    String response = "<!DOCTYPE html><html><head><title>MPU6050 Sensor Data</title></head><body><h1>ESP8266 MPU6050 Sensor Readings</h1><p>Acceleration (g):</p><ul><li>X: ";
    response += a.acceleration.x;
    response += "</li><li>Y: ";
    response += a.acceleration.y;
    response += "</li><li>Z: ";
    response += a.acceleration.z;
    response += "</li></ul><p>Gyroscope (rad/sec):</p><ul><li>X: ";
    response += g.gyro.x;
    response += "</li><li>Y: ";
    response += g.gyro.y;
    response += "</li><li>Z: ";
    response += g.gyro.z;
    response += "</li></ul><p>Temperature (°C): ";
    response += temp.temperature;
    response += "</p></body></html>";

    // Send web page content
    client.println(response);
    client.stop();
    Serial.println("Client served.");
  }
}
