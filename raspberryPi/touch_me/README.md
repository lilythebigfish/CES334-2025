## Module 3: Touch Me, a Graphite Sensor Art Installation

This code works together with a raspberryPi 5, a graphite drawing, bluetooth speakers, and a correctly wired esp32 with a ultrasonic distance sensor and a DIY graphite sensor. 
When run correctly, it will sense when someone approaches the graphite drawing and calls out "touch me" until the viewer
the esp32 heart and anywhere on the graphite at the same time. When the viewer releases touch, it will call out "come back to me"
once before going back to call out "touch me".  

To test out your distance sensor and graphite sensor is wired correctly, I have provided two files distanceSensor.ino and 
graphiteSensor.ino to flash onto your esp32. Once those sensors is confirmed, flash esp32Code.ino on your esp32.  

Then, on your raspberryPi, place reboot-starter.service in your etc/systemd/system folder. Daemon-reload your systemctl, then 
enable reboot-starter.service. Ensure your raspberryPi is connected to Yale Wireless and that your ESP32 is registered in the 
network. Connect to your bluetooth speaker once, and set it to the default audio output. 

When you restart your RaspberryPi, the program should run automatically, with audio playing through your bluetooth speaker.
