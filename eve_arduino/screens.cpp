#include "screens.h"

int current_screen = 0;
void display_current_screen(uint8_t r, uint8_t g, uint8_t b) {
  // Serial.println("Displaying current screen");
  // switch (current_screen) {
  //   case 0:
  //     load_coolcuremain();
  //     Serial.println("Displaying splash screen");
  //     display_splash_screen();
  //     break;
  //   case 1:
  //     Serial.println("Displaying store screen");
  //     display_main_screen(68.0, 54.0, 57, "STORE");
  //     break;
  //   case 2:
  //     Serial.println("Displaying dry screen");
  //     display_main_screen(68.0, 54.0, 57, "DRY");
  //     break;
  //   case 3:
  //     Serial.println("Displaying cure screen");
  //     display_main_screen(68.0, 54.0, 57, "CURE");
  //     break;
  //   case 4:
  //     Serial.println("Displaying store sliders");
  //     display_sliders("STORE");
  //     break;
  //   case 5:
  //     Serial.println("Displaying dry sliders");
  //     display_sliders("DRY");
  //     break;
  //   case 6:
  //     Serial.println("Displaying cure sliders");
  //     display_sliders("CURE");  
  //     break;
  //   case 7:
      Serial.println("Displaying wheel");
      display_wheel_screen(r, g, b);
  //     break;
  // }
}

void display_wheel_screen(uint8_t r, uint8_t g, uint8_t b) {
  clear_screen(COLOR_GRAY);
  display_image(IMAGE_WHEEL, 25, 25);
  draw_rect(331, 112, 133, 80, r, g, b);
  flush_screen_commands();
}

void display_main_screen(float temperature, float dew_point, int humidity, char* state_text) {
  Color color;
  if (strcmp(state_text, "STORE") == 0) {
    color = COLOR_GREEN;
  } else if (strcmp(state_text, "DRY") == 0) {
    color = COLOR_PURPLE;
  } else if (strcmp(state_text, "CURE") == 0) {
    color = COLOR_BLUE;
  }
  clear_screen(color);

    // Start Button
  if (strcmp(state_text, "STORE") == 0) {
    display_image(IMAGE_BTN_START, 419, 20);
  } else if (strcmp(state_text, "DRY") == 0 || strcmp(state_text, "CURE") == 0) {
    display_image(IMAGE_BTN_START, 372, 20);
    display_image(IMAGE_BTN_RESET, 426, 20);
    display_image(IMAGE_BTN_NEXT, 448, 199);
  }
  
  // Settings Button
  display_image(IMAGE_GEARS, 435, 229);
  
  // Cool Cure Logo
  display_image(IMAGE_COOLCUREBOTTOM, 234, 238);

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
  

  if (strcmp(state_text, "STORE") == 0) {
    display_text(FONT_55W_12PX_A, "Start", 428, 68, COLOR_BLACK);
  } else if (strcmp(state_text, "DRY") == 0 || strcmp(state_text, "CURE") == 0) {
    display_text(FONT_55W_12PX_A, "Start", 382, 68, COLOR_BLACK);
    display_text(FONT_55W_12PX_A, "Reset", 434, 68, COLOR_BLACK);
  }
  
  if (strcmp(state_text, "STORE") == 0) {
    display_text(FONT_75W_39PX_A, state_text, 328, 119, COLOR_WHITE);
  } else if (strcmp(state_text, "DRY") == 0) {
    display_text(FONT_75W_39PX_A, "DRY", 234, 119, COLOR_WHITE);
    display_text(FONT_75W_39PX_A, "CYCLE", 332, 119, COLOR_WHITE);
  } else if (strcmp(state_text, "CURE") == 0) {
    display_text(FONT_75W_39PX_A, "CURE", 206, 119, COLOR_WHITE);
    display_text(FONT_75W_39PX_A, "CYCLE", 332, 119, COLOR_WHITE);
  }

  if (strcmp(state_text, "DRY") == 0) {
    display_text(FONT_65W_30PX_AN, "Duration", 225, 162, COLOR_BLACK);
    display_text(FONT_65W_30PX_AN, "3d", 351, 162, COLOR_BLACK);
    display_text(FONT_65W_30PX_AN, "23:59", 393, 162, COLOR_BLACK);
    display_text(FONT_55W_21PX_A, "Advance", 288, 196, COLOR_BLACK);
    display_text(FONT_55W_21PX_A, "to", 376, 196, COLOR_BLACK);
    display_text(FONT_55W_21PX_A, "Dry", 401, 196, COLOR_BLACK);
  } else if (strcmp(state_text, "CURE") == 0) {
    display_text(FONT_65W_30PX_AN, "Duration", 225, 162, COLOR_BLACK);
    display_text(FONT_65W_30PX_AN, "3d", 351, 162, COLOR_BLACK);
    display_text(FONT_65W_30PX_AN, "23:59", 393, 162, COLOR_BLACK);
    display_text(FONT_55W_21PX_A, "Advance", 281, 196, COLOR_BLACK);
    display_text(FONT_55W_21PX_A, "to", 369, 196, COLOR_BLACK);
    display_text(FONT_55W_21PX_A, "Store", 394, 196, COLOR_BLACK);
  }
  
  flush_screen_commands();
}

void display_sliders(char* state_text) {
  // Set background color to purple
  Color color;
  if (strcmp(state_text, "STORE") == 0) {
    color = COLOR_GREEN;
  } else if (strcmp(state_text, "DRY") == 0) {
    color = COLOR_PURPLE;
  } else if (strcmp(state_text, "CURE") == 0) {
    color = COLOR_BLUE;
  }
  clear_screen(color);
  
  // Temperature display and slider
  display_text(FONT_65W_84PX_N, "68.0", 15, 23, COLOR_WHITE);
  display_text(FONT_65W_40PX_F, "F", 188, 58, COLOR_BLACK);
  display_text(FONT_55W_18PX_A, "Temperature", 25, 96, COLOR_BLACK);
  display_slider(32, 128, 160, 11, 24, 100);
  
  // Days display and slider
  if (strcmp(state_text, "DRY") == 0 || strcmp(state_text, "CURE") == 0) {
    display_text(FONT_65W_84PX_N, "1", 238, 29, COLOR_WHITE);
    display_text(FONT_55W_18PX_A, "Days", 247, 97, COLOR_BLACK);
    display_slider(247, 129, 120, 11, 41, 100);
  }
  
  // Dew point display and slider
  display_text(FONT_65W_84PX_N, "54.0", 12, 153, COLOR_WHITE);
  display_text(FONT_65W_40PX_F, "F", 185, 176, COLOR_BLACK);
  display_text(FONT_55W_18PX_A, "Dew", 25, 216, COLOR_BLACK);
  display_text(FONT_55W_18PX_A, "Point", 67, 216, COLOR_BLACK);
  display_slider(31, 249, 160, 11, 24, 100);
  
  // Hours display and slider
  if (strcmp(state_text, "DRY") == 0 || strcmp(state_text, "CURE") == 0) {
    display_text(FONT_65W_84PX_N, "20", 235, 153, COLOR_WHITE);
    display_text(FONT_55W_18PX_A, "Hours", 247, 219, COLOR_BLACK);
    display_slider(247, 248, 120, 11, 0, 100);
  }
  
  // Return text
  display_text(FONT_55W_12PX_A, "Return", 432, 55, COLOR_BLACK);
  
  if (strcmp(state_text, "DRY") == 0) {
    display_text(FONT_75W_19PX_A, "DRY", 432, 96, COLOR_WHITE);
    display_text(FONT_75W_19PX_A, "CYCLE", 410, 121, COLOR_WHITE);
    display_text(FONT_75W_19PX_A, "SETTINGS", 380, 147, COLOR_WHITE);
  } else if (strcmp(state_text, "CURE") == 0) {
    display_text(FONT_75W_19PX_A, "CURE", 421, 96, COLOR_WHITE);
    display_text(FONT_75W_19PX_A, "CYCLE", 410, 121, COLOR_WHITE);
    display_text(FONT_75W_19PX_A, "SETTINGS", 380, 147, COLOR_WHITE);
  } else if (strcmp(state_text, "STORE") == 0) {
    display_text(FONT_75W_19PX_A, "STORE", 409, 96, COLOR_WHITE);
    display_text(FONT_75W_19PX_A, "SETTINGS", 382, 121, COLOR_WHITE);
  }
  
  if (strcmp(state_text, "DRY") == 0 || strcmp(state_text, "CURE") == 0) {
    drawRectangle(400, 189, 19, 20, COLOR_WHITE);
    drawRectangle(401, 191, 15, 15, COLOR_GRAY);
    display_text(FONT_55W_18PX_A, "Slope", 426, 192, COLOR_BLACK);
    drawRectangle(400, 224, 19, 20, COLOR_WHITE);
    // drawRectangle(401, 226, 15, 15, COLOR_GRAY);
    display_text(FONT_55W_18PX_A, "Step", 426, 227, COLOR_BLACK);
  }
  
  // Display the Cannatrol logo
  display_image(IMAGE_BTN_RETURN, 427, 7);
  
  flush_screen_commands();
}


void display_splash_screen() {
  clear_screen(COLOR_GRAY);
  display_image(IMAGE_CANNATROL, 121, 24);
  display_image(IMAGE_COOLCUREMAIN, 93, 86);
  display_text(FONT_65W_84PX_N, "3", 224, 178, COLOR_WHITE);
  flush_screen_commands();
}