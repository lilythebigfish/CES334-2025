#include <WiFi.h>
#include <WiFiUdp.h>

// ====================== WIFI / UDP ======================
const char* WIFI_SSID = "YOUR_WIFI";
const char* WIFI_PASS = "YOUR_PASSWORD";
const char* PI_IP     = "192.168.1.20";   // <-- your Pi's IP
const uint16_t PI_PORT = 4210;

WiFiUDP udp;

// ====================== PIN DEFINITIONS ======================
const int trigPin = 14;         // Ultrasonic TRIG pin
const int echoPin = 12;         // Ultrasonic ECHO pin (use a 5V->3.3V divider!)
int capSensePin = 13;           // ESP32 capacitive touch pin (must be T-capable)
int touchedCutoff = 65;         // Baseline value (determined by testing)

// ====================== ULTRASONIC CONSTANTS ======================
#define SOUND_SPEED 0.034       // cm per microsecond
#define ENTER_DISTANCE_CM 100.0 // Enter state if <= 100 cm

// ====================== VARIABLES ======================
long duration;
float distanceCm;
int touchValue;
bool near = false;
bool touchDetected = false;

// ====================== HELPERS ======================
void wifiConnect() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(300); }
}

void sendUDP(const char* state) {
  // JSON: {"state":"...", "cm":123.4, "touch":72, "near":1, "touched":0}
  char payload[128];
  snprintf(payload, sizeof(payload),
           "{\"state\":\"%s\",\"cm\":%.1f,\"touch\":%d,\"near\":%d,\"touched\":%d}",
           state, distanceCm, touchValue, near?1:0, touchDetected?1:0);
  udp.beginPacket(PI_IP, PI_PORT);
  udp.print(payload);
  udp.endPacket();
}

// ====================== SETUP ======================
void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  wifiConnect();          // connect Wi-Fi
  udp.begin(0);           // ephemeral local port
  Serial.println("System Started...");
}

// ====================== MAIN LOOP ======================
void loop() {
  // Keep Wi-Fi connected
  if (WiFi.status() != WL_CONNECTED) wifiConnect();

  // ---- READ ULTRASONIC ----
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000); // Timeout to prevent hang
  if (duration > 0) {
    distanceCm = (duration * SOUND_SPEED) / 2.0;
  } else {
    distanceCm = 9999; // No detection
  }

  // ---- DETERMINE NEAR OR OUT OF RANGE ----
  near = (distanceCm <= ENTER_DISTANCE_CM);

  // ---- READ TOUCH ----
  touchValue = touchRead(capSensePin);
  touchDetected = (touchValue != touchedCutoff);  // your requested logic

  // ---- STATE & PRINT ----
  const char* stateStr;
  if (!near) {
    stateStr = "OUT_OF_RANGE";
  } else if (near && !touchDetected) {
    stateStr = "NEAR_NO_TOUCH";
  } else { // near && touchDetected
    stateStr = "TOUCHING";
  }

  Serial.print("Distance(cm): ");
  Serial.print(distanceCm, 1);
  Serial.print(" | Touch: ");
  Serial.print(touchValue);
  Serial.print(" | State: ");
  Serial.println(stateStr);

  // ---- SEND UDP ----
  sendUDP(stateStr);

  delay(100); // ~10 Hz
}