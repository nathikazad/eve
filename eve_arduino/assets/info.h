#ifndef ASSETS_INFO_H
#define ASSETS_INFO_H
#include "asset.h"
enum Fonts {
    FONT_55W_12PX_A,
    FONT_55W_16PX_AN,
    FONT_55W_18PX_A,
    FONT_55W_21PX_A,
    FONT_55W_30PX_A,
    FONT_65W_30PX_AN,
    FONT_65W_64PX_N,
    FONT_65W_84PX_N,
    FONT_75W_19PX_A,
    FONT_75W_29PX_A,
    FONT_75W_39PX_A,
    FONT_65W_40PX_F,
    FONT_65W_40PX_N,
};

enum Images {
    IMAGE_BTN_NEXT,
    IMAGE_BTN_PAUSE,
    IMAGE_BTN_RESET,
    IMAGE_BTN_RETURN,
    IMAGE_BTN_START,
    IMAGE_CANNATROL,
    IMAGE_COOLCUREMAIN,
    IMAGE_GEARS,
    IMAGE_COOLCUREBOTTOM,
};

void load_assets();
#endif
