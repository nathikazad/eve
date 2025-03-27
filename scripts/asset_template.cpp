#include "../eve/Eve2_81x.h"
#include "FS.h"
#include "LittleFS.h"

bool loadAssetFromLittleFS(const char* filename, uint32_t ram_g_addr) {
  Serial.printf("Loading image file: %s to RAM_G address: %u\n", filename, ram_g_addr);
  
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
  Serial.printf("File size: %u bytes\n", fileSize);
  
  // Buffer for reading the file in chunks
  const size_t bufferSize = 1024; // Adjust based on your ESP8266 memory constraints
  uint8_t buffer[bufferSize];
  
  size_t bytesRead = 0;
  uint32_t currentAddr = ram_g_addr;
  
  while (bytesRead < fileSize) {
    size_t chunkSize = file.read(buffer, bufferSize);
    if (chunkSize == 0) {
      break; // End of file or error
    }
    
    // Write this chunk to EVE RAM_G
    WriteBlockRAM(currentAddr, buffer, chunkSize);
    
    // Update counters
    bytesRead += chunkSize;
    currentAddr += chunkSize;
    
    // Optional: Print progress
    Serial.printf("Loading progress: %u/%u bytes (%d%%)\n", 
                 bytesRead, fileSize, (bytesRead * 100) / fileSize);
  }
  
  file.close();
  
  if (bytesRead == fileSize) {
    Serial.println("Image loaded successfully");
    return true;
  } else {
    Serial.printf("Error: Only read %u of %u bytes\n", bytesRead, fileSize);
    return false;
  }
}