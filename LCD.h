#ifndef LCD_H
#define LCD_H

#include <avr/io.h>  //zou mss mooi zijn als die uiteindelijk 1 keer word geinclude in de main
#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"
#include "libraries/AdafruitSD/Adafruit_ImageReader.h"

#define SD_CS   4 // SD card select pin
#define TFT_DC  9 // TFT display/command pin
#define TFT_CS 10 // TFT select pin

#define BLOCK_SIZE 16
#define LENGTH 320
#define WIDTH 240

#define P2_STATUS_LOCATION 6        // plek waar "P2" op de statusbar komt te staan

#define BG_COLOR 0x2184             // background color
#define P1_COLOR ILI9341_BLUE       // kleur van "P1" text op statusbar
#define P2_COLOR ILI9341_YELLOW     // kleur van "P2" text op statusbar

class LCD {
    public:
        LCD();
        void drawWall(uint8_t x, uint8_t y);
        void drawBaseMap();
        void drawStatusBar();
        void updateLives(uint8_t p);
    
    private:
};
#endif