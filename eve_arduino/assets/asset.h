#ifndef ASSET_H
#define ASSET_H

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

bool initEve();
bool loadAssetFromLittleFS(const char* filename, uint32_t ram_g_addr);
void displayTextWithImage();
void displayImage(Image image, int x, int y);
void displayText(Font font, const char* text, int x, int y, int color);
#endif