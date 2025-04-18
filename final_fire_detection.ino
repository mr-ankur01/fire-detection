#define BLYNK_TEMPLATE_ID "TMPL37zrPytjb"
#define BLYNK_TEMPLATE_NAME "Fire Detection"
#define BLYNK_AUTH_TOKEN "IWH1V1Yp_wnZJh4fyj72GviStTce5q0f"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Replace with your WiFi credentials
char ssid[] = "";
char pass[] = "";

#define FLAME_SENSOR_PIN D1  // GPIO5
#define MQ2_SENSOR_PIN A0    // Analog pin

bool flameAlertSent = false;
bool smokeAlertSent = false;

BlynkTimer timer;

void sendSensorData() {
  int smokeLevel = analogRead(MQ2_SENSOR_PIN);
  int flameStatus = digitalRead(FLAME_SENSOR_PIN); // LOW means flame detected
  Serial.println(smokeLevel);
  // Send data to Blynk app
  Blynk.virtualWrite(V1, smokeLevel);
  Blynk.virtualWrite(V0, flameStatus == LOW ? 255 : 0); // LED ON if flame detected

  // Fire alert
  if (flameStatus == LOW && !flameAlertSent) {
    Blynk.logEvent("fire_detected", "Fire Detected!");
    Serial.println("Fire Detected!");
    flameAlertSent = true;
  } else if (flameStatus == HIGH) {
    flameAlertSent = false;
  }

  // Smoke alert
  if (smokeLevel > 500 && !smokeAlertSent) {
    Blynk.logEvent("smoke_detected", "Smoke Detected!");
    Serial.println("Smoke Detected!");
    smokeAlertSent = true;
  } else if (smokeLevel <= 500) {
    smokeAlertSent = false;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting....");
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(MQ2_SENSOR_PIN, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, sendSensorData);  // Every 2 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}
