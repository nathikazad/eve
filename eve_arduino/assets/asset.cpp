#include "../config.h"
#include "../eve/Eve2_81x.h"
#include "asset.h"
#include "info.h"
#include "FS.h"
#include "LittleFS.h"

bool initEve() {
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

// Display text and image
void displayTextWithImage() {
  Send_CMD(CMD_DLSTART);                   // Start a new display list
  Send_CMD(CLEAR_COLOR_RGB(0, 180, 0));      // Black background
  Send_CMD(CLEAR(1, 1, 1));                // Clear screen
  
  Image image = images[IMAGE_BTN_START];
  // Calculate position to center the image
  int x = (Display_Width() - image.width) / 2;
  int y = (Display_Height() - image.height) / 2 - 30; // Offset a bit to make room for text
  
  displayImage(image, x, y);
  // Display text
  Send_CMD(COLOR_RGB(255, 255, 255));      // White color for text
  Cmd_Text(Display_Width() / 2, y + image.height + 20, 28, OPT_CENTER, "Case Image Demo");
  Cmd_Text(Display_Width() / 2, y + image.height + 50, 22, OPT_CENTER, "Touch the screen to test coordinates");
  
  Font font = fonts[FONT_65W_84PX_N_46];
  displayText(font, "68", 22, 20, 255);
  
  // End the display list
  Send_CMD(DISPLAY());
  Send_CMD(CMD_SWAP);
  UpdateFIFO();
  Wait4CoProFIFOEmpty();
}

void displayImage(Image image, int x, int y) {
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


void displayText(Font font, const char* text, int x, int y, int color) {
  uint32_t fontDataAddr = 47696;
  // 2. Set up bitmap parameters
  Send_CMD(BITMAP_SOURCE(fontDataAddr));
  Send_CMD(BITMAP_LAYOUT(L1, 8, 62));  // Format L1, stride 8, height 62
  Send_CMD(BITMAP_SIZE(NEAREST, BORDER, BORDER, 64, 62));  // Width 64, height 62

  // 3. Now set the font
  Send_CMD(COLOR_RGB(255, 255, 255));
  Cmd_SetFont2(0, fontDataAddr, 46);  // Handle 0, address, first char '.'
  Cmd_Text(22, 20, 0, 0, "68");
}
