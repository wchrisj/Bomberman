#ifndef BOMBER_H
#define BOMBER_H
#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"
#include "libraries/Game_logic/character.h"
#include "libraries/Game_logic/bomb.h"
#include "libraries/Nunchuk/nunchuk.h"
#include "LCD.h"
#include "map.h"

#define TFT_CS 10 // TFT select pin
#define TFT_DC  9 // TFT display/command pin

typedef
    enum STATUS{
        notReady,
        waiting,
        checking,
        playing
    }
status_t;

void gameTimerInit();
void draw();
void showLives();

extern Adafruit_ILI9341 tft;
extern Character localCharacter;
extern Character externCharacter;
extern NunchukInput* nunchuk;
extern Map mapGenerator;

#define PE 0x20     // port expander

#define BOARD_A     // rev 1
//#define BOARD_B   // rev 2

#ifdef BOARD_A
#define P1_1 1
#define P1_2 2
#define P1_3 3
#define P2_1 0
#define P2_2 7
#define P2_3 6
#endif

#ifdef BOARD_B
#define P1_1 2
#define P1_2 1
#define P1_3 0
#define P2_1 5
#define P2_2 6
#define P2_3 7
#endif

#endif