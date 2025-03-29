#include "info.h"


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
