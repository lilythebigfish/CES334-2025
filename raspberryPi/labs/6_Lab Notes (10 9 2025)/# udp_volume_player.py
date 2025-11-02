# udp_volume_player.py
import socket, threading, time
import pygame

AUDIO_FILE = "/Users/lilylin/Downloads/jingle.mp3"
PORT = 4210        
SMOOTH = 0.7   

# Start UDP listener
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", PORT))
sock.setblocking(False)

latest_vol = 0.0

def map_adc_to_vol(adc):
    # adc 0..4095 -> 0.0..1.0
    v = max(0, min(4095, adc))
    return v / 4095.0

def listener():
    global latest_vol
    buf = b""
    while True:
        try:
            data, _ = sock.recvfrom(1024)
            buf += data
            while b"\n" in buf:
                line, buf = buf.split(b"\n", 1)
                try:
                    adc = int(line.strip())
                    target = map_adc_to_vol(adc)
                    latest_vol = SMOOTH*latest_vol + (1-SMOOTH)*target
                except ValueError:
                    pass
        except BlockingIOError:
            time.sleep(0.01)

# Audio init
pygame.mixer.init()          
pygame.mixer.music.load(AUDIO_FILE)
pygame.mixer.music.set_volume(0.0)
pygame.mixer.music.play(-1)  

# Start UDP thread
threading.Thread(target=listener, daemon=True).start()

print(f"Listening UDP on port {PORT}. Playing {AUDIO_FILE}. Ctrl+C to quit.")
try:
    while True:
        pygame.mixer.music.set_volume(latest_vol)
        time.sleep(0.05)
except KeyboardInterrupt:
    pass
finally:
    pygame.mixer.music.stop()
    pygame.mixer.quit()
    sock.close()
