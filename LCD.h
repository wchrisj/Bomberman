#ifndef LCD_H
#define LCD_H

#include <avr/io.h>  //zou mss mooi zijn als die uiteindelijk 1 keer word geinclude in de main
#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"


#define TFT_DC 9
#define TFT_CS 10

#define BLOCK_SIZE 16
#define LENGTH 320
#define WIDTH 240

#define BG_COLOR ILI9341_BLACK
#define CRATE_SHADOW 0x5100
#define CRATE_FILLING 0xA200
#define CRATE_HIGHLIGHT 0xC280
#define SKIN 0xF5F1
#define PANTS 0x0377
#define BROWN 0x9220

class LCD {
    public:
        LCD();
        void drawAir(uint8_t x, uint8_t y);
        void drawWall(uint8_t x, uint8_t y);
        void drawCrate(uint8_t x, uint16_t y);
        void drawMap();
        void drawPlayer(uint8_t x, uint16_t y, uint16_t c);
        void statusBar();
    
    private:
};
#endif