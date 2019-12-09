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

class LCD {
    public:
        LCD(Adafruit_ILI9341 *tft);
        void drawWall(Adafruit_ILI9341 *tft, uint8_t x, uint8_t y);
        void drawMap(Adafruit_ILI9341 *tft);
        void statusBar(Adafruit_ILI9341 *tft);
    
    private:
};
#endif