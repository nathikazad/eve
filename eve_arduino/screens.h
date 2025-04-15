#ifndef SCREENS_H
#define SCREENS_H

#include "eve/eve.h"
#include "eve/assets.h"

extern int current_screen;

void display_current_screen(uint8_t r, uint8_t g, uint8_t b);
void display_main_screen(float temperature, float dew_point, int humidity, char* state_text);
void display_sliders(char* state_text);
void display_splash_screen();
void display_wheel_screen(uint8_t r, uint8_t g, uint8_t b);
#endif