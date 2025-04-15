#include "assets.h"
const Font fonts[] = {
    {0, 3628, FONT_TYPE_ALPHABETS, 65}, // 55W_12PX_A
    {3628, 7028, FONT_TYPE_ALPHA_NUMERIC, 37}, // 55W_16PX_AN
    {10656, 5020, FONT_TYPE_ALPHABETS, 65}, // 55W_18PX_A
    {15676, 5484, FONT_TYPE_ALPHABETS, 65}, // 55W_21PX_A
    {21160, 8036, FONT_TYPE_ALPHABETS, 65}, // 55W_30PX_A
    {42940, 4756, FONT_TYPE_NUMBERS, 46}, // 65W_64PX_N
    {47696, 6100, FONT_TYPE_NUMBERS, 46}, // 65W_84PX_N
    {53796, 5252, FONT_TYPE_ALPHABETS, 65}, // 75W_19PX_A
    {59048, 7804, FONT_TYPE_ALPHABETS, 65}, // 75W_29PX_A
    {66852, 20564, FONT_TYPE_ALPHABETS, 65}, // 75W_39PX_A
    {41040, 264, FONT_TYPE_ONLY_F, 70}, // 65W_40PX_F
    {41304, 1636, FONT_TYPE_NUMBERS, 46}, // 65W_40PX_N
    {29196, 11844, FONT_TYPE_ALPHA_NUMERIC, 37}, // 65W_30PX_AN
};

const Image images[] = {
    {87868, 4232, 46, 46, IMAGE_FORMAT_ARGB1555}, // BTN_PAUSE
    {108664, 4232, 46, 46, IMAGE_FORMAT_ARGB1555}, // BTN_RETURN
    {112896, 4140, 46, 45, IMAGE_FORMAT_ARGB1555}, // BTN_START
    {216348, 1650, 240, 55, IMAGE_FORMAT_L1}, // CANNATROL
    {218000, 42280, 302, 70, IMAGE_FORMAT_ARGB1555}, // COOLCUREMAIN
    {140340, 3280, 40, 41, IMAGE_FORMAT_ARGB1555}, // GEARS
    {143620, 806, 208, 31, IMAGE_FORMAT_L1}, // COOLCUREBOTTOM
    {92100, 4232, 46, 46, IMAGE_FORMAT_ARGB1555}, // BTN_RESET
    {144428, 800, 20, 20, IMAGE_FORMAT_ARGB1555}, // BTN_NEXT
    {121268, 4232, 46, 46, IMAGE_FORMAT_ARGB1555}, // BTN_WIFI
    {117036, 4232, 46, 46, IMAGE_FORMAT_ARGB1555}, // BTN_HOME
    {125500, 4140, 46, 45, IMAGE_FORMAT_ARGB1555}, // BTN_TEMP
    {129640, 4232, 46, 46, IMAGE_FORMAT_ARGB1555}, // BTN_BLUETOOTH
    {133872, 4232, 46, 46, IMAGE_FORMAT_ARGB1555}, // BTN_LANG
    {148660, 4232, 46, 46, IMAGE_FORMAT_ARGB1555}, // BTN_SUBMIT_ONMDPI
    {152892, 4140, 46, 45, IMAGE_FORMAT_ARGB1555}, // BTN_FACTORY_ONMDPI
    {160000, 80000, 200, 200, IMAGE_FORMAT_ARGB1555}, // WHEEL
};



void load_assets() {
    load_asset_from_littlefs("/fonts/55w_12px_A_65.raw", 0);
    load_asset_from_littlefs("/fonts/55w_16px_AN_37.raw", 3628);
    load_asset_from_littlefs("/fonts/55w_18px_A_65_1.raw", 10656);
    load_asset_from_littlefs("/fonts/55w_21px_A_65.raw", 15676);
    load_asset_from_littlefs("/fonts/55w_30px_A_65_1.raw", 21160);
    load_asset_from_littlefs("/fonts/65w_64px_N_46.raw", 42940);
    load_asset_from_littlefs("/fonts/65w_84px_N_46.raw", 47696);
    load_asset_from_littlefs("/fonts/75w_19px_A_65.raw", 53796);
    load_asset_from_littlefs("/fonts/75w_29px_A_65.raw", 59048);
    load_asset_from_littlefs("/fonts/75w_39px_A_65.raw", 66852);
    load_asset_from_littlefs("/fonts/65w_40px_F_70.raw", 41040);
    load_asset_from_littlefs("/fonts/65w_40px_N_46_1.raw", 41304);
    load_asset_from_littlefs("/fonts/65w_30px_AN_37.raw", 29196);
    load_asset_from_littlefs("/images/BTN_Pause.raw", 87868);
    load_asset_from_littlefs("/images/BTN_Return.raw", 108664);
    load_asset_from_littlefs("/images/BTN_Start.raw", 112896);
    load_asset_from_littlefs("/images/Cannatrol.raw", 216348);
    load_asset_from_littlefs("/images/CoolCureMain.raw", 218000);
    load_asset_from_littlefs("/images/Gears.raw", 140340);
    load_asset_from_littlefs("/images/CoolCureBottom.raw", 143620);
    load_asset_from_littlefs("/images/BTN_Reset_onmdpi.raw", 92100);
    load_asset_from_littlefs("/images/BTN_SM_Next_onmdpi.raw", 144428);
    load_asset_from_littlefs("/images/BTN_Wifi_onmdpi.raw", 121268);
    load_asset_from_littlefs("/images/BTN_Home_onmdpi.raw", 117036);
    load_asset_from_littlefs("/images/BTN_Temp_onmdpi.raw", 125500);
    load_asset_from_littlefs("/images/BTN_Bluetooth_onmdpi.raw", 129640);
    load_asset_from_littlefs("/images/BTN_Lang_onmdpi.raw", 133872);
    load_asset_from_littlefs("/images/BTN_Submit_onmdpi.raw", 148660);
    load_asset_from_littlefs("/images/BTN_Factory_onmdpi.raw", 152892);
}


void load_wheel() {
    load_asset_from_littlefs("/images/wheel.raw", 160000);
}