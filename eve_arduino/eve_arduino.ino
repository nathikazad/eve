#include "Arduino.h"
#include <SPI.h>
#include "eve/MatrixEve2Conf.h"
#include "eve/Eve2_81x.c"
#include "eve/hal.cpp"  
#include "assets/asset.cpp"
#include "assets/info.cpp" 
#include "screens.h"

#include "FS.h"
#include "LittleFS.h"

// Variables to track touch coordinates
int16_t lastX = -32768;
int16_t lastY = -32768;
unsigned long lastTouchCheck = 0;

int current_screen = 0;

void display_current_screen() {
  switch (current_screen) {
    case 0:
      Serial.println("Displaying splash screen");
      display_splash_screen();
      break;
    case 1:
      Serial.println("Displaying store screen");
      display_main_screen(68.0, 54.0, 57, "STORE");
      break;
    case 2:
      Serial.println("Displaying dry screen");
      display_main_screen(68.0, 54.0, 57, "DRY");
      break;
    case 3:
      Serial.println("Displaying cure screen");
      display_main_screen(68.0, 54.0, 57, "CURE");
      break;
    case 4:
      Serial.println("Displaying sliders");
      display_sliders("STORE");
      break;
    case 5:
      Serial.println("Displaying sliders");
      display_sliders("DRY");
      break;
    case 6:
      Serial.println("Displaying sliders");
      display_sliders("CURE");  
      break;
  }
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
  init_eve();
  int asset_load_start = millis();
  load_assets();  
  int asset_load_end = millis();
  Serial.printf("Asset load time: %d ms\n", asset_load_end - asset_load_start);
  display_current_screen();
}

void loop() {
  // Check touch coordinates every 100ms
  unsigned long currentMillis = millis();
  if (currentMillis - lastTouchCheck >= 100) {
    lastTouchCheck = currentMillis;
    
    // Read touch coordinates
    uint32_t touchXY = rd32(RAM_REG + REG_TOUCH_SCREEN_XY);
    int16_t touchX = touchXY >> 16;
    int16_t touchY = touchXY & 0xFFFF;

    // If coordinates have changed, print them
    if (touchX == -32768 && touchX == -32768) {
      // Touch released
      if (lastX != -32768 && lastY != -32768) {
        Serial.println("Touch released");
        lastX = -32768;
        lastY = -32768;
        current_screen++;
        if (current_screen > 6) {
          current_screen = 0;
        }
        display_current_screen();
      }
    } else if (touchX != lastX || touchY != lastY) {
      Serial.print("Touch X: ");
      Serial.print(touchX);
      Serial.print(", Y: ");
      Serial.println(touchY);
      // Update last known position
      lastX = touchX;
      lastY = touchY;
      
    }
      
    
  }
}

