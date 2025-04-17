#include "driver/Eve2_81x.h"
#include "eve.h"
#include "assets.h"
#include <math.h>

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

void draw_rect(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b) {
  Send_CMD(COLOR_RGB(r, g, b));
  Send_CMD(BEGIN(RECTS));
  Send_CMD(VERTEX2F(x*16, y*16));
  Send_CMD(VERTEX2F(x*16 + width*16, y*16 + height*16));
  Send_CMD(END());
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

void draw_rectangle(int x, int y, int width, int height, Color color) {
    // Slope and Step buttons
  if (color == COLOR_GRAY) {
    Cmd_FGcolor(0xB1B1B1);
  } else {
    Cmd_FGcolor(0xFFFFFF);
  }
  Cmd_Button(x, y, width, height, 27, 0, " ");
}

void draw_circle(int x_center, int y_center, int radius, Color color) {
  // Set the color for the circle border
  uint8_t r, g, b;
  get_color_rgb(color, &r, &g, &b);
  Send_CMD(COLOR_RGB(r, g, b));
  
  // Set line width for the border (adjust as needed)
  Send_CMD(LINE_WIDTH(16));  // Line width is in 1/16 pixel units, so 16 = 1 pixel
  
  // Begin drawing a line strip (connected lines)
  Send_CMD(BEGIN(LINE_STRIP));
  
  // Calculate points around the circle
  const int num_segments = 60;  // More segments = smoother circle
  float theta = 0;
  float step = 2.0f * 3.14159f / num_segments;  // Full circle in radians
  
  // Draw the first point to connect back to the end
  int x = x_center + radius * cos(0);
  int y = y_center + radius * sin(0);
  Send_CMD(VERTEX2F(x * 16, y * 16));  // Coordinates multiplied by 16
  
  // Draw the circle by connecting points around the circumference
  for (int i = 0; i <= num_segments; i++) {
    theta = i * step;
    x = x_center + radius * cos(theta);
    y = y_center + radius * sin(theta);
    Send_CMD(VERTEX2F(x * 16, y * 16));  // Coordinates multiplied by 16
  }
  
  // End the primitive
  Send_CMD(END());
}