// eve_arduino.ino
#include "Arduino.h"
#include "eve/init.cpp"
#include "LittleFS.h"
#include "screens.h"
#include <math.h>

#define EveChipSelect_PIN     D7  // PB1
#define EveAudioEnable_PIN    D1  // PD1
#define EvePDN_PIN            D6  // PB0
#define EveInterrupt_PIN      D5  // PC5

#define SCK D8
#define MISO D9
#define MOSI D10

#define TOUCH_SCREEN_WIDTH 480
#define TOUCH_SCREEN_HEIGHT 272
#define TOUCH_SCREEN_X_MAX 1024
#define TOUCH_SCREEN_Y_MAX 950

#define COLOR_WHEEL_RADIUS 90  // Color wheel radius in pixels
#define MIN_BRIGHTNESS 0.15f   // Minimum brightness (15%) - note the 'f' suffix for float

uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;

// Function to calculate RGB values from color wheel coordinates
void calculateRGB(int16_t x0, int16_t y0, uint8_t* r, uint8_t* g, uint8_t* b) {
  // Calculate distance from center (radius)
  float radius = sqrt((float)(x0 * x0 + y0 * y0));
  float r_normalized = (radius < COLOR_WHEEL_RADIUS) ? radius / COLOR_WHEEL_RADIUS : 1.0f;
  
  // Ensure minimum brightness
  if (r_normalized < MIN_BRIGHTNESS) {
    r_normalized = MIN_BRIGHTNESS;
  }
  
  // Calculate angle in degrees
  float theta = (90.0f - (atan2((float)y0, (float)x0) * 180.0f / M_PI));
  // Ensure angle is within 0-360 range
  while (theta < 0.0f) theta += 360.0f;
  while (theta >= 360.0f) theta -= 360.0f;
  
  // Calculate R/G/B components
  float R = 0.0f;
  if (abs(theta) < 120.0f) {
    R = (2.0f - abs(theta) / 60.0f);
    if (R > 1.0f) R = 1.0f;
    if (R < 0.0f) R = 0.0f;
  } else if (theta > 300.0f) {
    R = (2.0f - abs(theta - 360.0f) / 60.0f);
    if (R > 1.0f) R = 1.0f;
    if (R < 0.0f) R = 0.0f;
  }
  
  float G = 0.0f;
  float theta_g = abs(theta - 120.0f);
  if (theta_g < 120.0f) {
    G = (2.0f - theta_g / 60.0f);
    if (G > 1.0f) G = 1.0f;
    if (G < 0.0f) G = 0.0f;
  }
  
  float B = 0.0f;
  float theta_b = abs(theta - 240.0f);
  if (theta_b < 120.0f) {
    B = (2.0f - theta_b / 60.0f);
    if (B > 1.0f) B = 1.0f;
    if (B < 0.0f) B = 0.0f;
  }
  
  // Adjust for brightness intensity
  float R_final = R * r_normalized;
  float G_final = G * r_normalized;
  float B_final = B * r_normalized;
  
  // Convert to uint8_t (0-255)
  *r = (uint8_t)(R_final * 255.0f);
  *b = (uint8_t)(G_final * 255.0f);
  *g = (uint8_t)(B_final * 255.0f);
}

void handleTouch(int16_t x, int16_t y) {
  // You can use the x,y coordinates to determine what to do
  
  uint16_t touch_x = x * TOUCH_SCREEN_WIDTH / TOUCH_SCREEN_X_MAX;
  uint16_t touch_y = TOUCH_SCREEN_HEIGHT - (y * TOUCH_SCREEN_HEIGHT / TOUCH_SCREEN_Y_MAX);

  uint16_t x0 = 130-touch_x;
  uint16_t y0 = 111-touch_y;
  
  // Calculate RGB values based on touch position
  uint8_t local_r, local_g, local_b;
  calculateRGB(x0, y0, &local_r, &local_g, &local_b);
  
  // Now you can use r, g, b values (0-255) for your color wheel
  Serial.printf("Touch at (%d, %d) -> RGB: (%d, %d, %d)\n", x0, y0, local_r, local_g, local_b);
  
  // Increment current_screen
  // current_screen++;
  // if (current_screen > 7) {
  //   current_screen = 0;
  // }
  
  // Display the new screen
  // if (xSemaphoreTake(spiMutex, portMAX_DELAY) == pdTRUE) {
    r = local_r;
    g = local_g;
    b = local_b;
    //   xSemaphoreGive(spiMutex);
    // }
    display_current_screen(r, g, b);
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
  load_wheel();
  display_current_screen(r, g, b);
}

void loop() {
  // Check if touch interrupt was triggered
  vTaskDelay(pdMS_TO_TICKS(10));
}

