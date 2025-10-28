#!/usr/bin/env python3
import socket, subprocess, os

PORT = 4210
FILE = "/home/student334/CES334-2025/raspberryPi/touch_me/audio/touch_me.mp3"

if not os.path.exists(FILE):
    raise SystemExit(f"Missing: {FILE}")

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", PORT))
print(f"Listening UDP :{PORT} — will play once on any packet")

while True:
    data, addr = sock.recvfrom(1024)
    print("RX from", addr, data.decode(errors="ignore"))
    subprocess.run(["cvlc", "--intf", "dummy", "--play-and-exit", FILE],
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
