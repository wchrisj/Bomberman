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

//#define BG_COLOR ILI9341_BLACK
#define BG_COLOR 0x01C0  // background kleur
#define CRATE_SHADOW 0x5100     // voor crate
#define CRATE_FILLING 0xA200
#define CRATE_HIGHLIGHT 0xC280
#define SKIN 0xF5F1             // voor player
#define PANTS 0x0377
#define BROWN 0x9220
#define PLAYER_1 0xF800         // mario rood voor player
#define PLAYER_2 0x0526         // luigi groen voor player
#define WALUIGI 0x8810
#define OUTER 0xF320    // donker oranje
#define MIDDLE 0xF520   // oranje
#define INNER 0xF6E6    // licht oranje

class LCD {
    public:
        LCD();
        void drawAir(uint8_t x, uint8_t y);
        void drawWall(uint8_t x, uint8_t y);
        void drawCrate(uint8_t x, uint16_t y);
        void drawBomb(uint8_t x, uint16_t y);
        void drawExplosion(uint8_t x, uint16_t y, uint8_t i);
        void drawMap();
        void drawPlayer(uint8_t x, uint16_t y, uint16_t c);
        void statusBar();
    
    private:
};
#endif


/*
drawExplosion()

            5
            1
    7   3   0   4   8
            2
            6
*/