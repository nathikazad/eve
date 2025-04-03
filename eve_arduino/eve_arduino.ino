// eve_arduino.ino
#include "Arduino.h"
#include "eve/init.cpp"
#include "LittleFS.h"

#define EveChipSelect_PIN     D7  // PB1
#define EveAudioEnable_PIN    D1  // PD1
#define EvePDN_PIN            D6  // PB0
#define EveInterrupt_PIN      D5  // PC5

#define SCK D8
#define MISO D9
#define MOSI D10

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  while(!Serial);
  delay(1000);
  Serial.println("Starting EVE demo with image display");

  Serial.println("Mounting LittleFS...");
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }
  init_eve(EvePDN_PIN, EveChipSelect_PIN, SCK, MISO, MOSI, EveAudioEnable_PIN, EveInterrupt_PIN);
}

void loop() {
  // Check if touch interrupt was triggered
  vTaskDelay(pdMS_TO_TICKS(10));
}

