#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <SPI.h> // Not strictly necessary for MPU6050, but may be required on some boards

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
  Serial.println("ssid","Password");
  WiFi.begin(ssid);
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
  
  if (client) {
    Serial.println("Client connected");
    while (client.connected()) {
      mpu.getEvent(&a, &g, &temp);
      
      // Prepare sensor data as a string
      String data = String(a.acceleration.x) + "\t" + 
                    String(a.acceleration.y) + "\t" + 
                    String(a.acceleration.z) + "\t" + 
                    String(g.gyro.x) + "\t" + 
                    String(g.gyro.y) + "\t" + 
                    String(g.gyro.z) + "\t" + 
                    String(temp.temperature) + "\n";
      
      // Send sensor data to the client
      client.print(data);
      
      delay(1000); // Wait for 1 second before sending the next data
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
