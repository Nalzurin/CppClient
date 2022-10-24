#pragma once
#ifndef GraphicsLib_h
#define GraphicsLib_h

#include <stdint.h>

#ifndef R_G_B
#define R_G_B(r,g,b) (((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) //RGB565: 5 red | 6 green | 5 blue
#endif

class GraphicsLib
{

private:
    int_least16_t width, height; //screen size
public:


    GraphicsLib(uint_least16_t w, uint_least16_t h) : width(w), height(h) {};
    GraphicsLib() : width(0), height(0) {};
    int_least16_t getWidth(void) { return width; };
    int_least16_t getHeight(void) { return height; };

    virtual void fillScreen(uint_least16_t color) = 0;
    virtual void drawPixel(int_least16_t x0, int_least16_t y0, uint_least16_t color) = 0;
    virtual void drawLine(int_least16_t x0, int_least16_t y0, int_least16_t x1, int_least16_t y1, uint_least16_t color) = 0;
    virtual void drawRect(int_least16_t x0, int_least16_t y0, int_least16_t w, int_least16_t h, uint_least16_t color) = 0;
    virtual void fillRect(int_least16_t x0, int_least16_t y0, int_least16_t w, int_least16_t h, uint_least16_t color) = 0;
    virtual void drawEllipse(int_least16_t x0, int_least16_t y0, int_least16_t r_x, int_least16_t r_y, uint_least16_t color) = 0;
    virtual void fillEllipse(int_least16_t x0, int_least16_t y0, int_least16_t r_x, int_least16_t r_y, uint_least16_t color) = 0;
    virtual int_least16_t drawChar(int_least16_t x, int_least16_t y, char c, uint_least16_t color, uint_least16_t bg, uint_least8_t size) = 0;
    virtual int_least16_t drawChar(int_least16_t x, int_least16_t y, unsigned char c, uint_least16_t color, uint_least16_t bg, uint_least8_t size) = 0;
    virtual int_least16_t drawText(int_least16_t x, int_least16_t y, const char* s, uint_least16_t color, uint_least16_t bg, uint_least8_t size) = 0;
    virtual void loadSprite(uint_least8_t index, int_least16_t width, int_least16_t height, char* data) = 0;
    virtual void showSprite(uint_least8_t index, uint_least16_t x, uint_least16_t y) = 0;

};

#endif //GraphicsLib_h
