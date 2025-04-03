#ifndef SCREENS_H
#define SCREENS_H

#include "assets/info.h"
#include "assets/asset.h"

void display_main_screen(float temperature, float dew_point, int humidity, char* state_text);
void display_sliders(char* state_text);
void display_splash_screen();
#endif