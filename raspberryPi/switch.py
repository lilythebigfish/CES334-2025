from gpiozero import Button

button = Button(14)

button.wait_for_press()
print('pushed!')
