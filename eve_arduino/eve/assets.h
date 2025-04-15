#ifndef ASSETS_H
#define ASSETS_H
#include "eve.h"
enum Fonts {
    FONT_55W_12PX_A,
    FONT_55W_16PX_AN,
    FONT_55W_18PX_A,
    FONT_55W_21PX_A,
    FONT_55W_30PX_A,
    FONT_65W_64PX_N,
    FONT_65W_84PX_N,
    FONT_75W_19PX_A,
    FONT_75W_29PX_A,
    FONT_75W_39PX_A,
    FONT_65W_40PX_F,
    FONT_65W_40PX_N,
    FONT_65W_30PX_AN,
};

enum Images {
    IMAGE_BTN_PAUSE,
    IMAGE_BTN_RETURN,
    IMAGE_BTN_START,
    IMAGE_CANNATROL,
    IMAGE_COOLCUREMAIN,
    IMAGE_GEARS,
    IMAGE_COOLCUREBOTTOM,
    IMAGE_BTN_RESET,
    IMAGE_BTN_NEXT,
    IMAGE_WHEEL,
};

void load_assets();
void load_wheel();
void load_coolcuremain();
#endif
