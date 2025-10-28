#!/usr/bin/env python3
import socket, json, time, subprocess, os

PORT = 4210
TOUCH_LOOP = "/home/pi/audio/touch_me.mp3"
COME_BACK  = "/home/pi/audio/come_back.mp3"

HEARTBEAT_TIMEOUT = 2.0
POST_RELEASE_DELAY = 1.0

# ---------- utilities ----------
def coerce_bool(v, keyname):
    """Interpret 1/0, True/False, or '1'/'0' strings safely."""
    if isinstance(v, bool):
        return v
    if isinstance(v, (int, float)):
        return bool(int(v))
    if isinstance(v, str):
        s = v.strip().lower()
        if s in ("1", "true", "yes", "y", "on"):  return True
        if s in ("0", "false", "no", "n", "off"): return False
    print(f"[WARN] Could not coerce '{keyname}'={v!r} to bool; treating as False")
    return False

player = None

def kill_player():
    global player
    if player and player.poll() is None:
        print("[AUDIO] kill_player()")
        player.terminate()
        try: player.wait(timeout=0.5)
        except subprocess.TimeoutExpired: player.kill()
    player = None

def play_once_async(path):
    global player
    if not os.path.exists(path):
        print(f"[AUDIO][WARN] missing file: {path}")
        return
    print(f"[AUDIO] play_once_async: {path}")
    kill_player()
    player = subprocess.Popen(
        ["cvlc", "--intf", "dummy", "--quiet", "--play-and-exit", path],
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL
    )

def play_once_blocking(path):
    if not os.path.exists(path):
        print(f"[AUDIO][WARN] missing file: {path}")
        return
    print(f"[AUDIO] play_once_blocking: {path}")
    subprocess.run(
        ["cvlc", "--intf", "dummy", "--quiet", "--play-and-exit", path],
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL
    )

def finished():
    """True if no player running."""
    return (player is None) or (player.poll() is not None)

# ---------- state ----------
near = False
touched = False
last_release = 0.0
last_packet = 0.0

def main():
    global near, touched, last_release, last_packet

    for p in (TOUCH_LOOP, COME_BACK):
        if not os.path.exists(p):
            print(f"[WARN] File not found: {p}")

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("", PORT))
    sock.setblocking(False)
    print(f"[OK] Listening UDP :{PORT}")

    try:
        while True:
            now = time.time()

            # ----- receive -----
            try:
                data, addr = sock.recvfrom(1024)
                last_packet = now
                raw = data.decode("utf-8", errors="ignore")
                print(f"[RX] {addr[0]}: {raw}")

                msg = json.loads(raw)

                # Accept either key name; coerce robustly
                n_val = msg.get("near",  msg.get("isNear", 0))
                t_val = msg.get("touched", msg.get("touch", 0))
                cm    = msg.get("cm", None)

                new_near   = coerce_bool(n_val, "near")
                new_touch  = coerce_bool(t_val, "touched/touch")

                # Log interpreted values
                print(f"[PARSE] near={int(new_near)} touch={int(new_touch)} cm={cm}")

                # --- transitions / decisions (simple & explicit) ---
                if not new_near:
                    if near != new_near:
                        print("[STATE] -> OUT_OF_RANGE: stopping audio")
                    kill_player()
                    last_release = 0.0

                else:  # NEAR
                    if new_touch:
                        if touched != new_touch or not finished():
                            print("[STATE] NEAR + TOUCHED: stop immediately")
                        kill_player()
                        last_release = 0.0
                    else:
                        # NEAR + NOT TOUCHED: ensure we keep calling
                        if finished():
                            print("[STATE] NEAR + NOT TOUCHED: playing touch loop once")
                            play_once_async(TOUCH_LOOP)

                        # detect release moment (old touched -> new not touched)
                        if touched and not new_touch:
                            last_release = now
                            print("[STATE] touch released: will play come_back in 1s")

                # commit
                near, touched = new_near, new_touch

            except BlockingIOError:
                pass
            except Exception as e:
                print("[ERR] bad packet:", e)

            # ----- heartbeat timeout -----
            if last_packet and (now - last_packet > HEARTBEAT_TIMEOUT):
                if not finished():
                    print("[HB] timeout: silencing")
                kill_player()

            # ----- post-release one-shot -----
            if last_release and (now - last_release >= 1.0) and near and not touched:
                print("[STATE] post-release: play come_back once")
                last_release = 0.0
                kill_player()
                play_once_blocking(COME_BACK)
                # then resume call if still near & not touched
                if near and not touched:
                    print("[STATE] resume calling")
                    play_once_async(TOUCH_LOOP)

            time.sleep(0.03)
    except KeyboardInterrupt:
        pass
    finally:
        kill_player()

if __name__ == "__main__":
    print("[INFO] Ensure VLC installed: sudo apt update && sudo apt install -y vlc")
    main()
