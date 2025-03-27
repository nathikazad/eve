#include "Arduino.h"
#include <SPI.h>
#include "eve/MatrixEve2Conf.h"
#include "eve/Eve2_81x.c"
#include "eve/hal.cpp"  
#include "assets/image_data.h" 
// #include "gen/assets.h" 
// #include "gen/assets.cpp" 

#include "FS.h"
#include "LittleFS.h"

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

// Variables to track touch coordinates
int16_t lastX = -1;
int16_t lastY = -1;
unsigned long lastTouchCheck = 0;

// Image dimensions from JSON metadata
#define IMAGE_WIDTH 181
#define IMAGE_HEIGHT 185
#define IMAGE_FORMAT ARGB1555
#define IMAGE_STRIDE 362
#define IMAGE_ADDR 0  // Start address in RAM_G

#define EveChipSelect_PIN     D7  // PB1
#define EveAudioEnable_PIN    D1  // PD1
#define EvePDN_PIN            D6  // PB0

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting EVE demo with image display");

  Serial.println("Mounting LittleFS...");
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }
  
  // List files in root directory and 2 levels deep
  listDir(LittleFS, "/", 2);
  
  // Initialize EVE hardware
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
  
  // Load image data directly from the header file to RAM_G
  Serial.printf("Loading image data (%u bytes) to RAM_G...\n", case_181x185_ARGB1555_raw_len);
  WriteBlockRAM(IMAGE_ADDR, case_181x185_ARGB1555_raw, case_181x185_ARGB1555_raw_len);
  Serial.println("Image data loaded to RAM_G");
  // loadAssetsIntoRAM();
  
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

// Display text and image
void displayTextWithImage() {
  Send_CMD(CMD_DLSTART);                   // Start a new display list
  Send_CMD(CLEAR_COLOR_RGB(0, 0, 0));      // Black background
  Send_CMD(CLEAR(1, 1, 1));                // Clear screen
  
  // Calculate position to center the image
  int x = (Display_Width() - IMAGE_WIDTH) / 2;
  int y = (Display_Height() - IMAGE_HEIGHT) / 2 - 30; // Offset a bit to make room for text
  
  // Use Cmd_SetBitmap to set up the bitmap
  Cmd_SetBitmap(IMAGE_ADDR, IMAGE_FORMAT, IMAGE_WIDTH, IMAGE_HEIGHT);
  
  // Draw the bitmap at the calculated position
  Send_CMD(BEGIN(BITMAPS));
  Send_CMD(VERTEX2F(x * 16, y * 16));      // Position scaled by 16
  Send_CMD(END());
  
  // Display text
  Send_CMD(COLOR_RGB(255, 255, 255));      // White color for text
  Cmd_Text(Display_Width() / 2, y + IMAGE_HEIGHT + 20, 28, OPT_CENTER, "Case Image Demo");
  Cmd_Text(Display_Width() / 2, y + IMAGE_HEIGHT + 50, 22, OPT_CENTER, "Touch the screen to test coordinates");
  
  // End the display list
  Send_CMD(DISPLAY());
  Send_CMD(CMD_SWAP);
  UpdateFIFO();
  Wait4CoProFIFOEmpty();
}

