#include <ESP32Servo.h>
#include <Stepper.h>

Stepper myStepper(2048, 12, 14, 27, 26);
Servo servo;

void setup() {
  myStepper.setSpeed(5);
  servo.attach(25);
  servo.write(0);
}

void slowSweep(int fromAngle, int toAngle, int stepDelay) {
  if (fromAngle < toAngle) {
    for (int pos = fromAngle; pos <= toAngle; pos++) {
      servo.write(pos);
      delay(stepDelay);
    }
  } else {
    for (int pos = fromAngle; pos >= toAngle; pos--) {
      servo.write(pos);
      delay(stepDelay);
    }
  }
}

void loop() {
  myStepper.step(4096);
  servo.write(90);
  delay(2000);
  slowSweep(90, 0, 30);
  delay(500);
  myStepper.step(-4096);
  servo.write(90);
  delay(2000);
  slowSweep(90, 0, 30);
  delay(500);
}