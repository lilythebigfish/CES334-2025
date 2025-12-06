#include <WiFi.h>
#include <WiFiUdp.h>

// WIFI and UDP DEFINITIONS
const char* WIFI_SSID = "yale wireless";
const char* WIFI_PASS = "";
const char* PI_IP     = "10.67.74.76";
const uint16_t PI_PORT = 4210;

WiFiUDP udp;

// PIN DEFINITIONS
const int trigPin = 14;   // Ultrasonic TRIG pin
const int echoPin = 12;   // Ultrasonic ECHO pin (level-shift to 3.3V!)
int capSensePin   = 13;   // ESP32 capacitive touch pin
int touchedCutoff = 17;   // Your baseline

// ULTRASONIC CONSTANTS FOR DISTNACE SENSOR
#define SOUND_SPEED 0.034            // cm
const float ENTER_DISTANCE_CM = 100; // Enter when <= 100 cm
const float EXIT_DISTANCE_CM  = 110; // Exit when > 110 cm

// VARIABLES 
long duration;
float distanceCm;
int touchValue;
bool nearState = false;
bool touchDetected = false;

// DEBOUNCING VARIABLES 
float ema = 0.0f;
const float ALPHA = 0.25f;       // (higher = snappier)
int stableTouch = 0, stableNoTouch = 0;
const int TOUCH_DEBOUNCE_N = 3;  // require 3 consecutive TOUCH samples to flip
int stableNear = 0, stableNoNear = 0;
const int NEAR_DEBOUNCE_N = 5;  // require 5 consecutive NEAR samples to flip

// HELPERS
void wifiConnect() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(300); }
}

void sendUDP(const char* state) {
  char payload[128];
  snprintf(payload, sizeof(payload),
           "{\"state\":\"%s\",\"cm\":%.1f,\"touch\":%d,\"near\":%d,\"touched\":%d}",
           state, distanceCm, touchValue, nearState?1:0, touchDetected?1:0);
  udp.beginPacket(PI_IP, PI_PORT);
  udp.print(payload);
  udp.endPacket();
}

// Ultrasonic read for distance sensor
float readDistanceCmSingle() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long d = pulseIn(echoPin, HIGH, 30000);             // timeout ~30 ms
  if (d <= 0) return 9999.0f;
  return (d * SOUND_SPEED) / 2.0f;
}

// Median filter to smooth out distance spikes when reading in
float median3(float a, float b, float c){
  if (a>b) { float t=a; a=b; b=t; }
  if (b>c) { float t=b; b=c; c=t; }
  if (a>b) { float t=a; a=b; b=t; }
  return b;
}

float readDistSmoothedCm(){
  float d1 = readDistanceCmSingle(); delay(8);
  float d2 = readDistanceCmSingle(); delay(8);
  float d3 = readDistanceCmSingle();
  float m = median3(d1,d2,d3);
  if (m <= 0 || m > 500) m = 9999.0f; // clamp outliers
  return m;
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  wifiConnect();
  udp.begin(0);

  // init EMA with first touch read
  ema = touchRead(capSensePin);
  Serial.println("System Started...");
}

// MAIN LOOP
void loop() {
  if (WiFi.status() != WL_CONNECTED) wifiConnect();

  // read in smoothed ultrasonic distance
  distanceCm = readDistSmoothedCm();

  // set NEAR detection
bool sensedNear = false;

// determine if in reactive distance
if (nearState) {
  // Currently in NEAR state, look for EXIT condition
  sensedNear = !(distanceCm > EXIT_DISTANCE_CM);  // true unless clearly far
} else {
  // Currently OUT_OF_RANGE, look for ENTER condition
  sensedNear = (distanceCm > 0 && distanceCm <= ENTER_DISTANCE_CM);
}

// Distance debouncing counting logic
if (sensedNear) {
  stableNear++;
  stableNoNear = 0;
} else {
  stableNoNear++;
  stableNear = 0;
}

// Only change state after 5 stable readings
if (!nearState && stableNear >= NEAR_DEBOUNCE_N) {
  nearState = true;
}
if (nearState && stableNoNear >= NEAR_DEBOUNCE_N) {
  nearState = false;
}


  // Touch debouncing logic
  touchValue = touchRead(capSensePin);
  ema = ALPHA * touchValue + (1.0f - ALPHA) * ema;
  bool sensedNow = ((int)ema <= touchedCutoff); // your requested logic

  if (sensedNow) { stableTouch++; stableNoTouch = 0; }
  else           { stableNoTouch++; stableTouch = 0; }

  //Only change state after 3 stable readings
  if (!touchDetected && stableTouch   >= TOUCH_DEBOUNCE_N) touchDetected = true;
  if ( touchDetected && stableNoTouch >= TOUCH_DEBOUNCE_N) touchDetected = false;

  // STATESS
  const char* stateStr;
  if (!nearState) {
    stateStr = "OUT_OF_RANGE";
  } else if (nearState && !touchDetected) {
    stateStr = "NEAR_NO_TOUCH";
  } else {
    stateStr = "TOUCHING";
  }

  // SERIAL PRINTS
  Serial.print("Distance(cm): ");
  Serial.print(distanceCm, 1);
  Serial.print(" | Touch raw: ");
  Serial.print(touchValue);
  Serial.print(" | Touch ema: ");
  Serial.print(ema, 1);
  Serial.print(" | State: ");
  Serial.println(stateStr);

  // SEND UDP
  sendUDP(stateStr);

  delay(50); // ~20 Hz updates
}