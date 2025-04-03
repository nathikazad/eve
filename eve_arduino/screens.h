#ifndef SCREENS_H
#define SCREENS_H

#include "eve/eve.h"
#include "eve/assets.h"

extern int current_screen;

void display_main_screen(float temperature, float dew_point, int humidity, char* state_text);
void display_sliders(char* state_text);
void display_splash_screen();
#endif