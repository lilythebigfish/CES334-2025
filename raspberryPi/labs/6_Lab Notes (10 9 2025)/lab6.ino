#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid      = "yale wireless"; // WPA2-Enterprise/captive portals won't work
const char* password  = "";

const int sensorPin   = 33;   // ADC1 pin
const int ledPin      = 26;   // onboard LED or external LED (LOW=off, HIGH=on)

const char* remoteIp  = "192.168.1.123"; // <-- laptop IP (same LAN)
const uint16_t remotePort = 4210;        // receiver port

WiFiUDP udp;

int lightInit = 0;

void setup() {
  Serial.begin(115200);
  delay(400);

  // Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");
  uint32_t t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    Serial.print('.');
    delay(250);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nWi-Fi failed (captive/enterprise or wrong SSID). Continuing offline.");
  } else {
    Serial.printf("\nConnected. IP: %s\n", WiFi.localIP().toString().c_str());
  }

  pinMode(ledPin, OUTPUT);
  analogSetPinAttenuation(sensorPin, ADC_11db);
  delay(50);
  lightInit = analogRead(sensorPin);
}

void loop() {
  // read + simple smoothing
  const int N = 8;
  uint32_t acc = 0;
  for (int i = 0; i < N; i++) { acc += analogRead(sensorPin); delayMicroseconds(200); }
  int lightVal = acc / N;               // 0..4095

  // LED logic (your original threshold)
  if (lightVal - lightInit < 50) digitalWrite(ledPin, HIGH);
  else                           digitalWrite(ledPin, LOW);

  // send UDP as a single ASCII line: "<value>\n"
  if (WiFi.status() == WL_CONNECTED) {
    udp.beginPacket(remoteIp, remotePort);
    udp.printf("%d\n", lightVal);
    udp.endPacket();
  }

  // debug
  static uint32_t last = 0;
  if (millis() - last > 500) {
    last = millis();
    Serial.printf("Sensor: %d\n", lightVal);
  }

  delay(50);
}
