#include "info.h"
const Font fonts[] = {
    {0, 3628, FONT_TYPE_ALPHABETS, 65}, // 55W_12PX_A
    {3628, 7028, FONT_TYPE_ALPHA_NUMERIC, 37}, // 55W_16PX_AN
    {10656, 5020, FONT_TYPE_ALPHABETS, 65}, // 55W_18PX_A
    {15676, 5484, FONT_TYPE_ALPHABETS, 65}, // 55W_21PX_A
    {21160, 8036, FONT_TYPE_ALPHABETS, 65}, // 55W_30PX_A
    {29196, 11844, FONT_TYPE_ALPHA_NUMERIC, 37}, // 65W_30PX_AN
    {42940, 4756, FONT_TYPE_NUMBERS, 46}, // 65W_64PX_N
    {47696, 6100, FONT_TYPE_NUMBERS, 46}, // 65W_84PX_N
    {53796, 5252, FONT_TYPE_ALPHABETS, 65}, // 75W_19PX_A
    {59048, 7804, FONT_TYPE_ALPHABETS, 65}, // 75W_29PX_A
    {66852, 20564, FONT_TYPE_ALPHABETS, 65}, // 75W_39PX_A
    {41040, 264, FONT_TYPE_ONLY_F, 70}, // 65W_40PX_F
    {41304, 1636, FONT_TYPE_NUMBERS, 46}, // 65W_40PX_N
};

const Image images[] = {
    {87416, 450, 15, 15, IMAGE_FORMAT_ARGB1555}, // BTN_NEXT
    {87868, 4232, 46, 46, IMAGE_FORMAT_ARGB1555}, // BTN_PAUSE
    {92100, 16562, 91, 91, IMAGE_FORMAT_ARGB1555}, // BTN_RESET
    {108664, 4232, 46, 46, IMAGE_FORMAT_ARGB1555}, // BTN_RETURN
    {112896, 4140, 46, 45, IMAGE_FORMAT_ARGB1555}, // BTN_START
    {117036, 1650, 240, 55, IMAGE_FORMAT_L1}, // CANNATROL
    {118688, 21140, 302, 70, IMAGE_FORMAT_PALETTED4444}, // COOLCUREMAIN
    {140340, 3280, 40, 41, IMAGE_FORMAT_ARGB1555}, // GEARS
    {143620, 806, 208, 31, IMAGE_FORMAT_L1}, // COOLCUREBOTTOM
};



void load_assets() {
    load_asset_from_littlefs("/fonts/55w_12px_A_65.raw", 0);
    load_asset_from_littlefs("/fonts/55w_16px_AN_37.raw", 3628);
    load_asset_from_littlefs("/fonts/55w_18px_A_65_1.raw", 10656);
    load_asset_from_littlefs("/fonts/55w_21px_A_65.raw", 15676);
    load_asset_from_littlefs("/fonts/55w_30px_A_65_1.raw", 21160);
    load_asset_from_littlefs("/fonts/65w_30px_AN_37.raw", 29196);
    load_asset_from_littlefs("/fonts/65w_64px_N_46.raw", 42940);
    load_asset_from_littlefs("/fonts/65w_84px_N_46.raw", 47696);
    load_asset_from_littlefs("/fonts/75w_19px_A_65.raw", 53796);
    load_asset_from_littlefs("/fonts/75w_29px_A_65.raw", 59048);
    load_asset_from_littlefs("/fonts/75w_39px_A_65.raw", 66852);
    load_asset_from_littlefs("/fonts/65w_40px_F_70.raw", 41040);
    load_asset_from_littlefs("/fonts/65w_40px_N_46_1.raw", 41304);
    load_asset_from_littlefs("/images/BTN_Next.raw", 87416);
    load_asset_from_littlefs("/images/BTN_Pause.raw", 87868);
    load_asset_from_littlefs("/images/BTN_Reset.raw", 92100);
    load_asset_from_littlefs("/images/BTN_Return.raw", 108664);
    load_asset_from_littlefs("/images/BTN_Start.raw", 112896);
    load_asset_from_littlefs("/images/Cannatrol.raw", 117036);
    load_asset_from_littlefs("/images/CoolCureMain.raw", 118688);
    load_asset_from_littlefs("/images/Gears.raw", 140340);
    load_asset_from_littlefs("/images/CoolCureBottom.raw", 143620);
}
