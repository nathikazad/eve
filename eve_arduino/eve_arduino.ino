// eve_arduino.ino
#include "Arduino.h"
#include "eve/init.cpp"
#include "LittleFS.h"
#include "screens.h"

#define EveChipSelect_PIN     D7  // PB1
#define EveAudioEnable_PIN    D1  // PD1
#define EvePDN_PIN            D6  // PB0
#define EveInterrupt_PIN      D5  // PC5

#define SCK D8
#define MISO D9
#define MOSI D10

void handleTouch(int16_t x, int16_t y) {
  // You can use the x,y coordinates to determine what to do
  
  // Increment current_screen
  current_screen++;
  if (current_screen > 6) {
    current_screen = 0;
  }
  
  // Display the new screen
  display_current_screen();
}

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
  init_eve(EvePDN_PIN, EveChipSelect_PIN, SCK, MISO, MOSI, EveAudioEnable_PIN, EveInterrupt_PIN, handleTouch);
  display_current_screen();
}

void loop() {
  // Check if touch interrupt was triggered
  vTaskDelay(pdMS_TO_TICKS(10));
}

