# Lab Notes (9/25/2025)

## ESP32 Programming: Arduino IDE Setup

You can use almost anything to program the ESP32, but the Arduino IDE is probably easiest for a number of reasons. It is assumed you will be *programming* the boards on your laptops, not the Pi. However, the ESP32 will be interfacing with both, which requires setup on both (potentially.)

ESP32 Programming: Arduino IDE Setup (see below note about board selection): [https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) 

### Testing your setup

To work (see and read/write from the ESP32 Dev board) you will need to select the correct board and port for your setup. The board will be the same always (see below), but the port will be potentially different every time, and certainly may be different from your team member. 

- For the board, be sure to select the **‘ESP32 Dev Module’** not the one shown in the above setup
- For the Port, choose the USB option appropriate for your OS. (If you use a Mac or windows machine, you *may* require drivers…) You can plug and unplug the ESP32 freely (unlike the Raspi) to see what appears. If nothing shows (on Mac or Windows) then you require drivers.
- ~~Choose File →ESP32→Hall Sensor, verify and upload the sketch to your board, then open the serial monitor in the IDE. You should see data squiggling on the plotter!~~

### **Troubleshooting**

If you get a message about Module Serial not found, use pip to install **pyserial** `pip install pyserial` on your system (via CLI) (or on Arch, just `pacman -S python-serial`)

You can tell whether you should use pip or pip3 based on the results of `which pip` which will tell you if your environment is configured to use pip for your python3 installation. (the path will be printed and will contain python3 and will not error)

If you get the error `A fatal error occurred: Could not open /dev/ttyUSB0, the port doesn't exist` you *may or may not be* on Arch Linux. You *may* need to `sudo chmod 666 /dev/ttyUSB0` or the serial equivalent on your system.

## ESP32 -> Pi Communication

First: check your groups on Pi:
`groups`
If not:

1.  in dialout group, add:
`sudo usermod -a -G dialout student334`
2. Reboot

Plug ESP32 microUSB → Pi Type A

Include Serial.print() and/or Serial.println() to communicatedata to the Pi
[https://docs.espressif.com/projects/esp-idf/en/latest/get-started/establish-serial-connection.html](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/establish-serial-connection.html)
Example with built-in sensor: [https://microcontrollerslab.com/esp32-built-in-hall-effect-sensor-with-arduino-ide/](https://microcontrollerslab.com/esp32-built-in-hall-effect-sensor-with-arduino-ide/)

# F25 Notes

- You should not have to hold “boot” or “reset” when plugging-in and programming the ESP32. Get a new board if this is the case.
- Check for drivers if you aren’t seeing the board when it’s plugged in
- Never plug in the board while it is being powered by the circuit (ie from the Pi)
- Before being reprogrammed, the ESP32 should spit out numbers (that you see when you open your serial monitor) that change when you touch the pins. This shows the board is working
- Language methods exist for all GPIO/pin functions on the ESP32 via the ESP32 board library.

# Lab Practicum: Pair programming the ESP32 to communicate with Raspi

- [x]  Setup [Arduino IDE](https://www.arduino.cc/en/software) on Laptop (see above)

*I had to download some packages for ESP32 in order for ESP32 Dev Module to show up as a board option.*

- [x]  (May not be necessary, but do anyway) Flash the below code to the ESP32 and open the serial monitor in the IDE to see and record the ESP32’s MAC address on the knowledge share page here: ‣

```cpp
/* 
It sometimes amazes me how much broken code is on the internet. I wish I hadn't written so much of it.
-sage
*/

#include "esp_mac.h"
uint8_t baseMac[6];

void setup() {
  Serial.begin(9600);
  // Get MAC address of the WiFi station interface
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
}
void loop() {
  Serial.print("Station MAC: ");
  for (int i = 0; i < 5; i++) {
    Serial.printf("%02X:", baseMac[i]);
  }
  Serial.printf("%02X\n", baseMac[5]);
  delay(1000);
}
```

- [ ]  Program ESP32 (DevkitC) for:
    - [Joystick](https://sankios.imediabank.com/2-axis-joystick): analog input
    
    [https://docs.google.com/document/d/1rE3Am-aW8afIp6Dm7R8-TgMWksuLyoXrJBMWPFoYg2Q/edit?tab=t.0](https://docs.google.com/document/d/1rE3Am-aW8afIp6Dm7R8-TgMWksuLyoXrJBMWPFoYg2Q/edit?tab=t.0)
    
    *I had to specified the baud rate in the serial monitor to match the one specified in Serial.begin() in order to get readable output*
    
    - Button: digital input
    - Switch: digital input
    
    (Hint: note that the “examples” folder under the File menu contains example files specific to the hardware you are using.)
    
- [x]  Communicate readings to your RasPi via serial (connected by micro USB)

*the serial reading code needs to be flashed onto the esp32 before plugging it into raspi*

https://protonestiot.medium.com/communication-between-esp32-and-raspberry-pi-using-uart-7b776786c08a

^our reference

```python
import time
import serial

ser = serial.Serial("/dev/ttyUSB0", 115200)

while True:
    # Read lines from serial until there's none left
    while ser.in_waiting:
        line = ser.readline().decode('utf-8').rstrip()
        print(line)
    time.sleep(0.01)
```

- [x]  Raspi should run a python script (or whatever you want) to read values from the USB port (e.g. [pyserial](https://pythonhosted.org/pyserial/) library) and print them (to the shell)
    - Hint: the ability of the Python script to read the incoming messages is based on the type of data being sent. When using found code, be sure of the expected data type - errors will result if the data is not as expected.
- [x]  Commit your working code from today to a subfolder in your 334 repo
- [ ]  Stretch: depending on what way you send your messages (Serial.write vs Serial.print(ln)) you will have to parse the data to do useful things. How can you discern one message from another?

[https://docs.google.com/document/d/1rE3Am-aW8afIp6Dm7R8-TgMWksuLyoXrJBMWPFoYg2Q/edit?tab=t.0](https://docs.google.com/document/d/1rE3Am-aW8afIp6Dm7R8-TgMWksuLyoXrJBMWPFoYg2Q/edit?tab=t.0)

[https://protonestiot.medium.com/communication-between-esp32-and-raspberry-pi-using-uart-7b776786c08a](https://protonestiot.medium.com/communication-between-esp32-and-raspberry-pi-using-uart-7b776786c08a)