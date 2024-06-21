import socket

# Define server IP address and port (same as configured in ESP8266)
SERVER_IP = '172.21.0.18'
SERVER_PORT = 80

def receive_data():
    # Create a socket object
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connect to the server
        client_socket.connect((SERVER_IP, SERVER_PORT))
        print(f'Connected to {SERVER_IP}:{SERVER_PORT}')

        while True:
            # Receive data from the server
            data = client_socket.recv(1024)  # Buffer size of 1024 bytes
            if not data:
                # If no data is received, break the loop
                break
            print(data.decode().strip())

    except Exception as e:
        print(f'Error: {e}')
    finally:
        # Close the connection
        client_socket.close()
        print('Connection closed')

if __name__ == "__main__":
    receive_data()
