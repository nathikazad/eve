enum FontType {
    FONT_TYPE_ALPHABETS,
    FONT_TYPE_NUMBERS,
    FONT_TYPE_ALPHA_NUMERIC,
    FONT_TYPE_ONLY_F,
};

class Font {
public:
    int memoryAddress;
    int size;
    FontType fontType;
    
    Font(int address, int sz, FontType type) : 
        memoryAddress(address), size(sz), fontType(type) {}
};

enum ImageFormat {
    IMAGE_FORMAT_ARGB1555,
    IMAGE_FORMAT_PALETTED4444,
    IMAGE_FORMAT_L1,
};

class Image {
public:
    const char* name;
    int memoryAddress;
    int size;
    int width;
    int height;
    ImageFormat format;
    
    Image(const char* n, int address, int sz, int w, int h, ImageFormat fmt) :
        name(n), memoryAddress(address), size(sz), width(w), height(h), format(fmt) {}
};

void loadAssetsIntoRAM();

enum Fonts {
    FONT_55W_12PX_A_65,
    FONT_55W_16PX_AN_37,
    FONT_55W_18PX_A_65,
    FONT_55W_21PX_A_65,
    FONT_55W_30PX_A_65,
    FONT_65W_30PX_AN_37,
    FONT_65W_64PX_N_46,
    FONT_65W_84PX_N_46,
    FONT_75W_19PX_A_65,
    FONT_75W_29PX_A_65,
    FONT_75W_39PX_A_65,
    FONT_65W_40PX_F_70,
    FONT_65W_40PX_N_46,
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

const Font fonts[] = {
    {0, 3628, FONT_TYPE_ALPHABETS},
    {3628, 7028, FONT_TYPE_ALPHA_NUMERIC},
    {10656, 5020, FONT_TYPE_ALPHABETS},
    {15676, 5484, FONT_TYPE_ALPHABETS},
    {21160, 8036, FONT_TYPE_ALPHABETS},
    {29196, 11844, FONT_TYPE_ALPHA_NUMERIC},
    {42940, 4756, FONT_TYPE_NUMBERS},
    {47696, 6100, FONT_TYPE_NUMBERS},
    {53796, 5252, FONT_TYPE_ALPHABETS},
    {59048, 7804, FONT_TYPE_ALPHABETS},
    {66852, 20564, FONT_TYPE_ALPHABETS},
    {41040, 264, FONT_TYPE_ONLY_F},
    {41304, 1636, FONT_TYPE_NUMBERS},
};

const Image images[] = {
    {"BTN_NEXT", 87416, 450, 15, 15, IMAGE_FORMAT_ARGB1555},
    {"BTN_PAUSE", 87868, 4232, 46, 46, IMAGE_FORMAT_ARGB1555},
    {"BTN_RESET", 92100, 16562, 91, 91, IMAGE_FORMAT_ARGB1555},
    {"BTN_RETURN", 108664, 4232, 46, 46, IMAGE_FORMAT_ARGB1555},
    {"BTN_START", 112896, 4140, 46, 45, IMAGE_FORMAT_ARGB1555},
    {"CANNATROL", 117036, 1650, 236, 55, IMAGE_FORMAT_L1},
    {"COOLCUREMAIN", 118688, 21140, 302, 70, IMAGE_FORMAT_PALETTED4444},
    {"GEARS", 140340, 3280, 40, 41, IMAGE_FORMAT_ARGB1555},
    {"COOLCUREBOTTOM", 143620, 806, 201, 31, IMAGE_FORMAT_L1},
};
