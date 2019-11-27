#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"


#define TFT_DC 9
#define TFT_CS 10

#define blok 16
#define lengte 320
#define breedte 240

class LCD {
    public:
        LCD();
        void drawWall(uint8_t x, uint8_t y);
        void drawMap();
    
    private:
};
#endif