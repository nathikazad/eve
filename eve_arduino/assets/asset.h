#ifndef ASSET_H
#define ASSET_H


// Buffer for reading the file in chunks
const size_t bufferSize = 1024; 
uint8_t buffer[bufferSize];

enum Color {
    COLOR_WHITE,
    COLOR_BLACK,
    COLOR_GREEN,
};

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
    int startChar;
    
    Font(int address, int sz, FontType type, int start_char) : 
        memoryAddress(address), size(sz), fontType(type), startChar(start_char) {}
};

enum ImageFormat {
    IMAGE_FORMAT_ARGB1555,
    IMAGE_FORMAT_PALETTED4444,
    IMAGE_FORMAT_L1,
};

class Image {
public:
    int memoryAddress;
    int size;
    int width;
    int height;
    ImageFormat format;
    
    Image(int address, int sz, int w, int h, ImageFormat fmt) :
        memoryAddress(address), size(sz), width(w), height(h), format(fmt) {}
};

extern const Font fonts[];
extern const Image images[];

bool init_eve();
bool load_asset_from_littlefs(const char* filename, uint32_t ram_g_addr);
void display_store_screen();
void clear_screen(Color color);
void flush_screen_commands();
void display_image(Image image, int x, int y);
void display_text(int font_index, const char* text, int x, int y, Color color);
void Cmd_SetFont2(uint32_t font, uint32_t ptr, uint32_t firstchar);
#endif