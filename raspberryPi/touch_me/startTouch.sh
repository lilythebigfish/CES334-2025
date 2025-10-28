#!/bin/bash

# ------------------------
# STARTUP SCRIPT FOR UDP AUDIO
# ------------------------

echo "[BOOT] Waiting for Bluetooth and Network..."
sleep 5   # Give time for Bluetooth stack and Wi-Fi to initialize

# ---- (Optional) Force reconnect to known Bluetooth speaker ----
# Replace XX_XX_XX_XX_XX_XX with your speaker MAC address (underscored format)
# Uncomment below if you want auto-BT connection
# echo -e "connect XX:XX:XX:XX:XX:XX\nquit" | bluetoothctl

# ---- (Optional) Set audio output to Bluetooth sink ----
# Replace sink name from: pactl list short sinks
# Uncomment when confirmed
# pactl set-default-sink bluez_output.XX_XX_XX_XX_XX_XX.a2dp-sink

# ---- Start UDP audio receiver ----
echo "[BOOT] Starting UDP Audio Script..."
/home/student334/CES334-2025/raspberryPi/touch_me/receiver.py
