# **Components required:**
* Compatible Arduino board (Mega/Due etc) 
* MPU6050 sensor 
* Breadboard 
* Jumper wires 

# **Circuit:**
![image](https://github.com/itskuldipsingh/MPU6050-Gyro-and-Accelerometer/assets/159125809/4447e1c6-8c13-44db-9d5c-ae430ece7d62)
* Sensor VCC to board 5V pin 
* Sensor GND to board GND 
* Sensor SLC to board SCL
* Sensor SDA to board SDA \
**Caution:** For optimal sensor readings, keep wires under 2 meters in length.
  
# **Write code and upload to Arduino board:**
* Download and install Arduino IDE from Linux app store/Microsoft app store/mac app store or [click here to download.](https://www.arduino.cc/en/software)
* Run Arduino IDE
* Sketch ⇒ Include Library ⇒ Manage Libraries... <br>
  ![image](https://github.com/itskuldipsingh/MPU6050-Gyro-and-Accelerometer/assets/159125809/b60ae3fd-0aa4-4790-924f-4b393ab13687)
* Search for 'Adafruit_MPU6050' and install latest version of it. <br>
  ![image](https://github.com/itskuldipsingh/MPU6050-Gyro-and-Accelerometer/assets/159125809/4e0446cc-c9a9-45d5-b55b-23d68284bc8b)
* Select Board ⇒ (select your board)
* Write [sketch.ino](https://github.com/itskuldipsingh/MPU6050-Gyro-and-Accelerometer/blob/main/Arduino/sketch.ino), compile and upload.
* Open serial monitor to view output (set serial monitor baud rate at 115200)
![image](https://github.com/itskuldipsingh/MPU6050-Gyro-and-Accelerometer/assets/159125809/006ce276-f800-4226-8a7b-6236f07376ab)

# **To see and save output of MPU6050 with arduino as a file in any Python environment follow [arduino_mpu_output](https://github.com/itskuldipsingh/Arduino/blob/main/MPU6050/arduino_mpu_output.md). For example we can see output of mpu6050 with arduino in [Vizard7](https://github.com/itskuldipsingh/Arduino/blob/main/MPU6050/vizard_mpu_output.md)**
