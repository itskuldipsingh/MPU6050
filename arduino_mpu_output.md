# Viewing MPU6050 Output with Arduino in Python
To observe the output of an MPU6050 sensor with Arduino in a Python environment, serial communication is essential. Follow these steps:
1. Ensure that the serial monitor output in the Arduino IDE is working correctly.
2. Close the Arduino IDE since a single serial communication cannot be shared between two environments.
3. Open any Python environment and create a new script [arduino_mpu_output.py](https://github.com/itskuldipsingh/MPU6050-Gyro-and-Accelerometer/blob/main/Arduino/vizard_mpu_output.py) and run it.

```python
import serial
import datetime
import os

serial_port = 'COM3'  # Change this to your Arduino's serial port
baud_rate = 115200 # Change this to your Arduino's serial communication baud rate

output_directory = "arduino_data"
os.makedirs(output_directory, exist_ok=True)

output_file = f"{output_directory}/{datetime.datetime.now().strftime('%d%b%Y%H%M')}.txt"

ser = serial.Serial(serial_port, baud_rate, timeout=1)

with open(output_file, 'w') as file:
    file.write("Timestamp\tAx\tAy\tAz\tGx\tGy\tGz\n")

    while True:
        try:
            data = ser.readline().decode("utf-8").strip()
            if data:
                timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                file.write(f"{timestamp}\t{data}\n")
                print(f"{timestamp}\t{data}")
        except KeyboardInterrupt:
            ser.close()
            break
```
   - The script will create a directory named `arduino_data` if it doesn't exist.
   - The script establishes serial communication with the Arduino, reads MPU6050 sensor data, and logs it.
   - The data is saved to a text file with a filename containing the timestamp of the start.
   - To stop the script, use a keyboard interrupt (Ctrl+C).
   - The script will close the serial connection before exiting.
