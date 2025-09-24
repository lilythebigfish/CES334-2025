from gpiozero import LED   
from gpiozero import Button
import time

ledS = LED(14)
ledL = LED(15)
button = Button(21, bounce_time=0.05)
switch = Button(20, bounce_time=0.05)
joystick = Button(19, bounce_time=0.05)
global mode
mode = {'mode': 0}





def change_mode():
    mode ['mode'] = (mode['mode']+1)%3
    print(f"mode is now {mode['mode']}")

print(f"mode is now {mode['mode']}")
button.when_pressed = change_mode



while True:
    if mode['mode'] == 0:
        if switch.is_pressed:
            ledS.on()
        else:
            ledS.off()
            
        if joystick.is_pressed:
            ledL.on()
        else:
            ledL.off()
        
    
    elif mode['mode'] == 1:
        joystick.when_pressed =lambda: print(f"joystick")
        if switch.is_pressed:
            print(f"left switch")
            switch.wait_for_release()
        else:
            print(f"right switch")
            switch.wait_for_press()
            
        
    else:
        joystick.when_pressed =lambda: print(f"  ",end="")
        if switch.is_pressed:
            print("-",end="")
            time.sleep(0.1)
        else:
            print(".",end="")
            time.sleep(0.1)
    