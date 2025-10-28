#include <WiFi.h>

const int trigPin = 14;
const int echoPin = 27;
const char* ssid = "yale wireless";
const char* password = "";

#define SOUND_SPEED 0.034  // cm per microsecond (speed of sound)

#define DISTANCE_THRESHOLD 70.0  // 1 meter = 100 cm

long duration;
float distanceCm;



void setup() {
  Serial.begin(115200);      // Start serial communication

  //wifi setup
  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

  //print distance
  pinMode(trigPin, OUTPUT);  // Set trigPin as output
  pinMode(echoPin, INPUT);   // Set echoPin as input
}

void loop() {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Trigger the sensor by setting the trigPin high for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin and calculate the duration
  duration = pulseIn(echoPin, HIGH, 30000); // Timeout to prevent lock-up

  // Calculate distance in cm
  distanceCm = (duration * SOUND_SPEED) / 2;

  // Print the actual distance
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  // Check if within 1 meter
  if (distanceCm > 0 && distanceCm <= DISTANCE_THRESHOLD) {
    Serial.println("Within distance");
  } else {
    Serial.println("Out of range");
  }

  Serial.println("---------------------"); // Separator for readability
  delay(500);
}