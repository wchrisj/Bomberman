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

#define BG_COLOR 0x2184 // background color

class LCD {
    public:
        LCD();
        void drawWall(uint8_t x, uint8_t y);
        void drawMap();
        void drawStatusBar();
        void updateLives(uint8_t p);
    
    private:
};
#endif