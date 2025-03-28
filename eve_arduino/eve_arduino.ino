#include "Arduino.h"
#include <SPI.h>
#include "eve/MatrixEve2Conf.h"
#include "eve/Eve2_81x.c"
#include "eve/hal.cpp"  
#include "assets/asset.cpp"
#include "assets/info.cpp" 

#include "FS.h"
#include "LittleFS.h"

// Variables to track touch coordinates
int16_t lastX = -1;
int16_t lastY = -1;
unsigned long lastTouchCheck = 0;

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
  initEve();
  loadAssetsIntoRAM();  
  // Display welcome text with image
  displayTextWithImage();
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
    
    // Check if touch is detected (coordinates of 0x8000 indicate no touch)
    if (touchX != 0x8000 && touchY != 0x8000) {
      // If coordinates have changed, print them
      if (touchX != lastX || touchY != lastY) {
        Serial.print("Touch X: ");
        Serial.print(touchX);
        Serial.print(", Y: ");
        Serial.println(touchY);
        
        // Update last known position
        lastX = touchX;
        lastY = touchY;
      }
    } else if (lastX != -32768 || lastY != -32768) {
      // Touch released
      Serial.println("Touch released");
      lastX = -32768;
      lastY = -32768;
    }
  }
}

