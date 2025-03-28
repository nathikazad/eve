#include "info.h"


void loadAssetsIntoRAM() {
    loadAssetFromLittleFS("/fonts/55w_12px_A_65.raw", 0);
    loadAssetFromLittleFS("/fonts/55w_16px_AN_37.raw", 3628);
    loadAssetFromLittleFS("/fonts/55w_18px_A_65_1.raw", 10656);
    loadAssetFromLittleFS("/fonts/55w_21px_A_65.raw", 15676);
    loadAssetFromLittleFS("/fonts/55w_30px_A_65_1.raw", 21160);
    loadAssetFromLittleFS("/fonts/65w_30px_AN_37.raw", 29196);
    loadAssetFromLittleFS("/fonts/65w_64px_N_46.raw", 42940);
    loadAssetFromLittleFS("/fonts/65w_84px_N_46.raw", 47696);
    loadAssetFromLittleFS("/fonts/75w_19px_A_65.raw", 53796);
    loadAssetFromLittleFS("/fonts/75w_29px_A_65.raw", 59048);
    loadAssetFromLittleFS("/fonts/75w_39px_A_65.raw", 66852);
    loadAssetFromLittleFS("/fonts/65w_40px_F_70.raw", 41040);
    loadAssetFromLittleFS("/fonts/65w_40px_N_46_1.raw", 41304);
    loadAssetFromLittleFS("/images/BTN_Next.raw", 87416);
    loadAssetFromLittleFS("/images/BTN_Pause.raw", 87868);
    loadAssetFromLittleFS("/images/BTN_Reset.raw", 92100);
    loadAssetFromLittleFS("/images/BTN_Return.raw", 108664);
    loadAssetFromLittleFS("/images/BTN_Start.raw", 112896);
    loadAssetFromLittleFS("/images/Cannatrol.raw", 117036);
    loadAssetFromLittleFS("/images/CoolCureMain.raw", 118688);
    loadAssetFromLittleFS("/images/Gears.raw", 140340);
    loadAssetFromLittleFS("/images/CoolCureBottom.raw", 143620);
}
