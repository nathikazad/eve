#include "asset.h"
#include "info.h"
#include <stdio.h>

void display_main_screen(Color color, float temperature, float dew_point, int humidity, char* state_text) {
  clear_screen(color);

  // Temperature display
  char temp_whole[3];
  char temp_frac[3];
  sprintf(temp_whole, "%d", (int)temperature);
  sprintf(temp_frac, ".%d", (int)((temperature - (int)temperature) * 10));
  display_text(FONT_65W_84PX_N, temp_whole, 22, 20, COLOR_WHITE);
  display_text(FONT_65W_40PX_N, temp_frac, 116, 50, COLOR_BLACK);
  display_text(FONT_65W_40PX_F, "F", 158, 50, COLOR_BLACK);
  display_text(FONT_55W_18PX_A, "Temperature", 32, 91, COLOR_BLACK);
  
  // Dew Point display
  char dew_whole[3];
  char dew_frac[3];
  sprintf(dew_whole, "%d", (int)dew_point);
  sprintf(dew_frac, ".%d", (int)((dew_point - (int)dew_point) * 10));
  display_text(FONT_65W_84PX_N, dew_whole, 25, 129, COLOR_WHITE);
  display_text(FONT_65W_40PX_N, dew_frac, 116, 160, COLOR_BLACK);
  display_text(FONT_65W_40PX_F, "F", 158, 160, COLOR_BLACK);
  display_text(FONT_55W_18PX_A, "Dew", 32, 206, COLOR_BLACK);
  display_text(FONT_55W_18PX_A, "Point", 79, 206, COLOR_BLACK);
  
  // Humidity display
  char humidity_str[4];
  sprintf(humidity_str, "%d%%", humidity);
  display_text(FONT_65W_30PX_AN, humidity_str, 32, 239, COLOR_BLACK);
  display_text(FONT_55W_30PX_A, "Humidity", 103, 239, COLOR_BLACK);
  
  // Start text
  display_text(FONT_55W_12PX_A, "Start", 428, 68, COLOR_BLACK);
  
  // Store text
  display_text(FONT_75W_39PX_A, state_text, 328, 119, COLOR_WHITE);
  
  // Start Button
  display_image(images[IMAGE_BTN_START], 419, 20);
  
  // Settings Button
  display_image(images[IMAGE_GEARS], 435, 229);
  
  // Cool Cure Logo
  display_image(images[IMAGE_COOLCUREBOTTOM], 234, 238);
  
  flush_screen_commands();
}

void display_store_screen() {
  display_main_screen(COLOR_GREEN, 68.0, 54.0, 57, "STORE");
}