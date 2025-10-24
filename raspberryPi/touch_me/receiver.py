import socket

# Set the port (must match ESP32 code)
UDP_PORT = 4210   # Use the same port you're sending to from ESP32

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", UDP_PORT))  # "" means listen on all available network interfaces

print(f"Listening for UDP packets on port {UDP_PORT}...\n")

while True:
    data, addr = sock.recvfrom(1024)  # Buffer size 1024 bytes
    print(f"Received from {addr}: {data.decode('utf-8')}")
