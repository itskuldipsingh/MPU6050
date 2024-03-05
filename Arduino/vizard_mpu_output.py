import serial
serial_port = 'COM3'  # Change this to your Arduino's serial port
baud_rate = 115200
ser = serial.Serial(serial_port, baud_rate, timeout=1)
while True:
	try:
		data = ser.readline().decode("utf-8").strip()
		if data:
			print(f"{data}")
	except KeyboardInterrupt:
		ser.close()
		break
