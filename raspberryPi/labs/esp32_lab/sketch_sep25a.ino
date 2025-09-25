// Define joystick pins
const int xPin = 25;  // X-axis connected to GPIO 32
const int yPin = 33;  // Y-axis connected to GPIO 33
const int buttonPin = 32;  // Button connected to GPIO 25
const int clickPin = 26;
const int switchPin = 27;


void setup() {
 // Initialize serial communication
 Serial.begin(115200);
  // Set button pin as input with pullup resistor
 pinMode(buttonPin, INPUT_PULLUP);
 pinMode(clickPin, INPUT_PULLDOWN);
 pinMode(switchPin, INPUT_PULLUP);
}


void loop() {
 // Read X and Y values (0 - 4095 on ESP32)
 int xValue = analogRead(xPin);
 int yValue = analogRead(yPin);
  // Read button state (0 when pressed, 1 when released)
 int buttonState = digitalRead(buttonPin);
 int clickState = digitalRead(clickPin);
 int switchState = digitalRead(switchPin);


 // Print values to Serial Monitor
 Serial.print("X: ");
 Serial.print(xValue);
 Serial.print("\tY: ");
 Serial.print(yValue);
 Serial.print("\tButton: ");
 Serial.print(buttonState);
 Serial.print("\tYellow Button: ");
 Serial.print(clickState);
 Serial.print("\tswitch: ");
 Serial.println(switchState);


 delay(100);
}
