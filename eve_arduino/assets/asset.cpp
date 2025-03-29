#include "../config.h"
#include "../eve/Eve2_81x.h"
#include "asset.h"
#include "FS.h"
#include "LittleFS.h"

bool init_eve() {
  pinMode(EvePDN_PIN, OUTPUT);
  digitalWrite(EvePDN_PIN, LOW);           // Reset condition
  delay(100);
  digitalWrite(EvePDN_PIN, HIGH);          // Release reset
  delay(100);
  
  pinMode(EveChipSelect_PIN, OUTPUT);
  digitalWrite(EveChipSelect_PIN, HIGH);   // Deselect EVE
  
  pinMode(EveAudioEnable_PIN, OUTPUT);
  digitalWrite(EveAudioEnable_PIN, LOW);   // Disable audio
  
  // Initialize SPI
  SPI.begin(SCK, MISO, MOSI, EveChipSelect_PIN);
  
  // Initialize EVE display
  FT81x_Init(DISPLAY_43, BOARD_EVE2, TOUCH_TPR);
  
  Serial.println("Display initialized");
  return true;
}

bool load_asset_from_littlefs(const char* filename, uint32_t ram_g_addr) {

  // Buffer for reading the file in chunks
  const size_t bufferSize = 1024; 
  uint8_t buffer[bufferSize];
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
      Serial.printf("Font address %d set to %d\n", 144, ram_g_addr+148);
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

void get_color_rgb(Color color, uint8_t* r, uint8_t* g, uint8_t* b) {
  switch (color) {
    case COLOR_WHITE:
      *r = 255;
      *g = 255;
      *b = 255;
      break;
    case COLOR_BLACK:
      *r = 0;
      *g = 0;
      *b = 0;
      break;
    case COLOR_GREEN:
      *r = 0;
      *g = 180;
      *b = 0;
      break;
  }
}

void clear_screen(Color color) {
  uint8_t r, g, b;
  get_color_rgb(color, &r, &g, &b);
  Send_CMD(CMD_DLSTART);                   // Start a new display list
  Send_CMD(CLEAR_COLOR_RGB(r, g, b));      // Green background
  Send_CMD(CLEAR(1, 1, 1));                // Clear screen
}

void flush_screen_commands() {
  Send_CMD(END());
  Send_CMD(DISPLAY());
  Send_CMD(CMD_SWAP);
  UpdateFIFO();
  Wait4CoProFIFOEmpty();
}

void display_image(Image image, int x, int y) {
  uint16_t format_value = ARGB1555;
  if (image.format == IMAGE_FORMAT_PALETTED4444) {
    format_value = PALETTED4444;
  } else if (image.format == IMAGE_FORMAT_L1) {
    format_value = L1;
  }
  Cmd_SetBitmap(image.memoryAddress, format_value, image.width, image.height);
  
  // Draw the bitmap at the calculated position
  Send_CMD(BEGIN(BITMAPS));
  Send_CMD(VERTEX2F(x * 16, y * 16));      // Position scaled by 16
  Send_CMD(END());
}

void Cmd_SetFont2(uint32_t font, uint32_t ptr, uint32_t firstchar)
{
  Send_CMD(0xFFFFFF3B);  // CMD_SETFONT2 command
  Send_CMD(font);        // font handle (0-31)
  Send_CMD(ptr);         // pointer to metrics in RAM_G
  Send_CMD(firstchar);   // first character ASCII value
}


void display_text(int font_index, const char* text, int x, int y, Color color) {
  Font font = fonts[font_index];
  uint8_t r, g, b;
  get_color_rgb(color, &r, &g, &b);
  Send_CMD(COLOR_RGB(r, g, b));
  Cmd_SetFont2(0, font.memoryAddress, font.startChar);
  Cmd_Text(x, y, 0, 0, text);
}
