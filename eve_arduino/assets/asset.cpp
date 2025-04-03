// asset.cpp
// This file contains the functions for loading assets into the EVE display and configuring the EVE display

#include "../config.h"
#include "../eve/Eve2_81x.h"
#include "asset.h"
#include "FS.h"
#include "LittleFS.h"

// Touch interrupt flag
volatile bool touch_interrupt = false;
int16_t last_touch_x = -32768;
int16_t last_touch_y = -32768;

// Interrupt handler function
void IRAM_ATTR handle_touch_interrupt() {
  touch_interrupt = true;
}

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

  // Configure touch interrupts
  init_eve_interrupts();
  
  Serial.println("Display initialized with touch interrupts configured");
  return true;
}

bool init_eve_interrupts() {

  // 1. Enable the TOUCH interrupt flag in the mask// Configure touch mode to ensure touch is active
  wr8(RAM_REG + REG_TOUCH_MODE, 3); // Continuous mode
  
  // Configure interrupt for touch
  // Clear any pending interrupts
  rd8(RAM_REG + REG_INT_FLAGS);
  
  // Set touch interrupt mask
  wr8(RAM_REG + REG_INT_MASK, 0x02);  // Enable TOUCH flag only (bit 1)
  
  // Enable global interrupt
  wr8(RAM_REG + REG_INT_EN, 0x01);    // Enable global interrupt
  
  // Configure INT_N as push-pull output
  uint16_t gpiox = rd16(RAM_REG + REG_GPIOX);
  
  // Configure INT_N as push-pull output (set bit 7)
  gpiox |= (1 << 7);
  wr16(RAM_REG + REG_GPIOX, gpiox);
  
  // Setup hardware interrupt pin
  pinMode(EveInterrupt_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EveInterrupt_PIN), handle_touch_interrupt, CHANGE);
  Serial.print("Attached EVE interrupt to pin ");
  Serial.println(EveInterrupt_PIN);
  
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

// Function to read and store touch coordinates
void read_touch_coordinates() {
  // Read touch coordinates
  uint32_t touchXY = rd32(RAM_REG + REG_TOUCH_SCREEN_XY);
  last_touch_x = touchXY >> 16;
  last_touch_y = touchXY & 0xFFFF;
  
  Serial.print("Touch X: ");
  Serial.print(last_touch_x);
  Serial.print(", Y: ");
  Serial.println(last_touch_y);
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
    case COLOR_PURPLE:
      *r = 126;
      *g = 87;
      *b = 196;
      break;
    case COLOR_BLUE:  
      *r = 0;
      *g = 151;
      *b = 206;
      break;
    case COLOR_YELLOW:
      *r = 255;
      *g = 255;
      *b = 127;
      break;
    case COLOR_GRAY:
      *r = 64;
      *g = 64;
      *b = 65;
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

void set_color(Color color) {
  uint8_t r, g, b;
  get_color_rgb(color, &r, &g, &b);
  Send_CMD(COLOR_RGB(r, g, b));
}

void display_image(int image_index, int x, int y) {
  Image image = images[image_index];
  uint16_t format_value = ARGB1555;
  if (image.format == IMAGE_FORMAT_PALETTED4444) {
    format_value = PALETTED4444;
  } else if (image.format == IMAGE_FORMAT_L1) {
    format_value = L1;
  }
  Cmd_SetBitmap(image.memoryAddress, format_value, image.width, image.height);
  set_color(COLOR_WHITE);
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
  set_color(color);
  Cmd_SetFont2(0, font.memoryAddress, font.startChar);
  Cmd_Text(x, y, 0, 0, text);
}

void display_slider(int x, int y, int width, int height, int value, int range) {
  set_color(COLOR_YELLOW);
  Cmd_FGcolor(0xFFFF00);
  Cmd_BGcolor(0x000000);
  Cmd_Slider(x, y, width, height, 0, value, range);
}

void drawRectangle(int x, int y, int width, int height, Color color) {
    // Slope and Step buttons
  if (color == COLOR_GRAY) {
    Cmd_FGcolor(0xB1B1B1);
  } else {
    Cmd_FGcolor(0xFFFFFF);
  }
  Cmd_Button(x, y, width, height, 27, 0, " ");
}