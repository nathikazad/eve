// asset.cpp
// This file contains the functions for loading assets into the EVE display and configuring the EVE display

#include "FS.h"
#include "LittleFS.h"
#include <FreeRTOS.h>

#include "driver/Eve2_81x.c"
#include "driver/hal.cpp"  
#include "eve.h"
#include "assets.cpp"
#include "screens.cpp"
#include "widgets.cpp"
#include "touch.cpp"

bool init_eve(int pd_pin, int cs_pin, int sck_pin, int miso_pin, int mosi_pin, int audio_pin, int interrupt_pin) {
  pinMode(pd_pin, OUTPUT);
  digitalWrite(pd_pin, LOW);           // Reset condition
  delay(100);
  digitalWrite(pd_pin, HIGH);          // Release reset
  delay(100);
  
  pinMode(cs_pin, OUTPUT);
  digitalWrite(cs_pin, HIGH);   // Deselect EVE
  
  pinMode(audio_pin, OUTPUT);
  digitalWrite(audio_pin, LOW);   // Disable audio
  
  // Initialize SPI
  SPI.begin(sck_pin, miso_pin, mosi_pin, cs_pin);

  HAL_Eve_Init(cs_pin, pd_pin);
  
  // Initialize EVE display
  FT81x_Init(DISPLAY_43, BOARD_EVE2, TOUCH_TPR);

  int asset_load_start = millis();
  load_assets();  
  int asset_load_end = millis();
  Serial.printf("Asset load time: %d ms\n", asset_load_end - asset_load_start);
  init_touch_interrupts(interrupt_pin);
  
  Serial.println("Display initialized with touch interrupts configured");
  display_current_screen();
  return true;
}

bool load_asset_from_littlefs(const char* filename, uint32_t ram_g_addr) {
  // Buffer for reading the file in chunks
  const size_t bufferSize = 1024*5; 
  uint8_t buffer[bufferSize];
  Serial.printf("File: %s, Address: %u, ", filename, ram_g_addr);
  
  if (!LittleFS.exists(filename)) {
    Serial.printf("Error: File %s does not exist\n", filename);
    return false;
  }
  
  File file = LittleFS.open(filename, "r");
  if (!file) {
    Serial.printf("Error: Failed to open file %s\n", filename);
    return false;
  }
  
  size_t fileSize = file.size();
  Serial.printf("Size: %u bytes, ", fileSize);

  
  size_t bytesRead = 0;
  uint32_t currentAddr = ram_g_addr;
  
  while (bytesRead < fileSize) {
    size_t chunkSize = file.read(buffer, bufferSize);
    if (chunkSize == 0) {
      break; // End of file or error
    }

    if (bytesRead == 0 && strncmp(filename, "/fonts", 6) == 0) {
      // set the font address to the ram_g_addr + 149.
      // Refer to 5.4.1 of FT81x/BT88x Series Programming Guide 
      // https://brtchip.com/wp-content/uploads/2023/09/BRT_AN_088_FT81x_BT88x-Programming-Guide.pdf

      uint32_t full_addr = ram_g_addr+148;
      buffer[144] = full_addr & 0xFF;          
      buffer[145] = (full_addr >> 8) & 0xFF;   
      buffer[146] = (full_addr >> 16) & 0xFF;  
      buffer[147] = (full_addr >> 24) & 0xFF;  
      // Serial.printf("Font address %d set to %d\n", 144, ram_g_addr+148);
    }
    
    // Write this chunk to EVE RAM_G
    WriteBlockRAM(currentAddr, buffer, chunkSize);
    
    // Update counters
    bytesRead += chunkSize;
    currentAddr += chunkSize;
    
    // // Optional: Print progress
    // Serial.printf("Loading progress: %u/%u bytes (%d%%)\n", 
    //              bytesRead, fileSize, (bytesRead * 100) / fileSize);
  }
  
  file.close();
  
  if (bytesRead == fileSize) {
    Serial.println("Success");
    return true;
  } else {
    Serial.printf("Error: Only read %u of %u bytes\n", bytesRead, fileSize);
    return false;
  }
}