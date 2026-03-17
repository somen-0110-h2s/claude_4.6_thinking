// ============================================================
//  ESP32 / Arduino Sketch
//  - Status LED blinks twice per second (every 500 ms)
//  - DHT11 temperature sampled every 5 seconds
//  - Both tasks run in parallel using millis()
// ============================================================

#include <DHT.h>
#include <Adafruit_Sensor.h>

// ---- Pin Definitions ----
#define DHT_PIN       4      // DHT11 data pin
#define DHT_MODEL     DHT11

// ESP32 does not always define LED_BUILTIN; GPIO 2 is the safe default
#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif
#define STATUS_LED    LED_BUILTIN

// ---- Task Intervals ----
#define LED_BLINK_MS    500    // Blink twice per second = toggle every 500 ms
#define TEMP_SAMPLE_MS  5000   // Sample temperature every 5 seconds

// ---- Objects ----
DHT sensor(DHT_PIN, DHT_MODEL);

// ---- Timing Trackers ----
unsigned long ledTimer  = 0;
unsigned long tempTimer = 0;

// ---- LED State ----
bool ledToggle = false;

// ============================================================
void setup() {
  Serial.begin(115200);
  while (!Serial) { ; } // Wait for serial port (optional, useful on some boards)

  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);

  sensor.begin();

  Serial.println("--------------------------------------------");
  Serial.println("  Sketch started!");
  Serial.println("  LED    : blinks every 500 ms");
  Serial.println("  Sensor : sampled every 5 seconds");
  Serial.println("--------------------------------------------");
}

// ============================================================
void loop() {
  unsigned long now = millis();

  // Task 1: Blink status LED twice per second
  if (now - ledTimer >= LED_BLINK_MS) {
    ledTimer   = now;
    ledToggle  = !ledToggle;
    digitalWrite(STATUS_LED, ledToggle);
  }

  // Task 2: Sample DHT11 every 5 seconds
  if (now - tempTimer >= TEMP_SAMPLE_MS) {
    tempTimer = now;

    float celsius    = sensor.readTemperature();
    float fahrenheit = sensor.readTemperature(true); // pass true for Fahrenheit

    if (isnan(celsius) || isnan(fahrenheit)) {
      Serial.println("[DHT11] Read failed. Check wiring.");
    } else {
      Serial.print("[DHT11] Temp: ");
      Serial.print(celsius);
      Serial.print(" C  /  ");
      Serial.print(fahrenheit);
      Serial.println(" F");
    }
  }
}
