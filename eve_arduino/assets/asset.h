#ifndef ASSET_H
#define ASSET_H

#include <Arduino.h> 

enum Color {
    COLOR_WHITE,
    COLOR_BLACK,
    COLOR_GREEN,
    COLOR_PURPLE,
    COLOR_BLUE,
    COLOR_YELLOW,
    COLOR_GRAY
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
void clear_screen(Color color);
void flush_screen_commands();
void display_image(int image_index, int x, int y);
void display_text(int font_index, const char* text, int x, int y, Color color);
void Cmd_SetFont2(uint32_t font, uint32_t ptr, uint32_t firstchar);
void display_slider(int x, int y, int width, int height, int value, int range);
void set_color(Color color);
void drawRectangle(int x, int y, int width, int height, Color color);
#endif