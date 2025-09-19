#!/bin/bash

DISPLAY=:0
cd /home/student334/CES334-2025/raspberryPi/vibration

# Start the HTTP server in background
/usr/bin/python3 -m http.server 8080 & firefox --kiosk http://0.0.0.0:8080/
