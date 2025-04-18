#ifndef EVE_H
#define EVE_H

#include <Arduino.h> 
#include <FreeRTOS.h>
#include <semphr.h>

// Add EVE display list macros that aren't in Eve2_81x.h
#define LINE_WIDTH(width) ((14UL<<24)|(((width)&4095UL)<<0))

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

// Function pointer type for touch callback
typedef void (*TouchCallback)(int16_t x, int16_t y);

extern const Font fonts[];
extern const Image images[];
extern volatile bool touch_interrupt;
extern int16_t last_touch_x;
extern int16_t last_touch_y;
extern TaskHandle_t touchTaskHandle;
extern SemaphoreHandle_t spiMutex;  // SPI mutex for thread safety


bool init_eve(int pd_pin, int cs_pin, int sck_pin, int miso_pin, int mosi_pin, int audio_pin, int interrupt_pin);
bool load_asset_from_littlefs(const char* filename, uint32_t ram_g_addr);
void clear_screen(Color color);
void flush_screen_commands();
void display_image(int image_index, int x, int y);
void display_text(int font_index, const char* text, int x, int y, Color color);
void Cmd_SetFont2(uint32_t font, uint32_t ptr, uint32_t firstchar);
void display_slider(int x, int y, int width, int height, int value, int range);
void set_color(Color color);
void draw_rectangle(int x, int y, int width, int height, Color color);
bool init_touch_interrupts(int interrupt_pin);
void read_touch_coordinates();
void touchHandlerTask(void* parameter);
void draw_rect(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b);
void draw_circle(int x_center, int y_center, int radius, Color color);
#endif