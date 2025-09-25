import time
import serial

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

while True:
    signal = ser.readline().decode('utf-8')
    print (signal)
    time.sleep(0.1)