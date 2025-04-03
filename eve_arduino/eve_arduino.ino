// eve_arduino.ino
#include "Arduino.h"
#include "eve/init.cpp"
#include "LittleFS.h"


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
  init_eve();
}

void loop() {
  // Check if touch interrupt was triggered
  vTaskDelay(pdMS_TO_TICKS(10));
}

