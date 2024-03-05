import serial
import datetime
import os

serial_port = 'COM3'  # Change this to your Arduino's serial port
baud_rate = 115200

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
