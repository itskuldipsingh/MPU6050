#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Create a sensor object
Adafruit_MPU6050 mpu;

// Variables to store sensor data
sensors_event_t a, g, temp; //a:acceleration(m/s^2) g:gyro(rad/sec) temp:temparature(celsius)

void setup() {
  Serial.begin(115200); // Initialize Serial Monitor

  // Initialize I2C communication
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(2000);
    }
  }
  Serial.println("MPU6050 Found!");
}

void loop() {
  mpu.getEvent(&a, &g, &temp);
  //Serial.print("accX:");
  Serial.print(a.acceleration.x);
  Serial.print("\t");
  //Serial.print("accY:");
  Serial.print(a.acceleration.y);
  Serial.print("\t");
  //Serial.print("accZ:");
  Serial.print(a.acceleration.z);
  Serial.print("\t");

  //Serial.print("gyroX:");
  Serial.print(g.gyro.x);
  Serial.print("\t");
  //Serial.print("gyroY:");
  Serial.print(g.gyro.y);
  Serial.print("\t");
  //Serial.print("gyroZ:");
  Serial.print(g.gyro.z);
  Serial.print("\t");

  //Serial.print("temp:");
  Serial.print(temp.temperature);
  Serial.println();

  // Add a delay to make the output readable
  delay(1000);
}
