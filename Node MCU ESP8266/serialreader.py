import serial

def read_serial_data(serial_port, baud_rate, timeout=1):
    try:
        ser = serial.Serial(serial_port, baud_rate, timeout=timeout)
        return ser
    except serial.SerialException as e:
        print(f"Error opening serial port {serial_port}: {e}")
        return None

def process_data(ser):
    while True:
        try:
            data = ser.readline().decode("utf-8").strip()
            if data:
                print(f"\t{data}")
        except KeyboardInterrupt:
            print("Keyboard interrupt received. Exiting...")
            ser.close()
            break
        except Exception as e:
            print(f"An error occurred: {e}")

def main():
    serial_port = '/dev/ttyUSB0'  # Linux (Change if Arduino port is different)
    serial_port = 'COM3'  # Windows (Change if Arduino port is different)
    baud_rate = 115200

    ser = read_serial_data(serial_port, baud_rate)
    if ser:
        process_data(ser)

if __name__ == "__main__":
    main()
