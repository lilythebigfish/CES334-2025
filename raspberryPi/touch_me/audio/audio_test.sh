#!/bin/bash
python3 - << 'EOF'
import subprocess
print("Playing audio...")
subprocess.run(["cvlc", "--intf", "dummy", "--play-and-exit", "/home/student334/CES334-2025/raspberryPi/touch_me/audio/touch_me.mp3"])
print("Done")
EOF