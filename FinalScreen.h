#ifndef FinalScreen_H
#define FinalScreen_H

#include <avr/io.h>  //zou mss mooi zijn als die uiteindelijk 1 keer word geinclude in de main
#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"
#include "libraries/Nunchuk/nunchuk.h"
#include "LCD.h"
#include "Waitingpage.h"

#define TBX_YOU 62  //Text Begin X-as
#define TBY_YOU 61
#define TBX_YOU_shadow 60
#define TBY_YOU_shadow 60

#define TBX_WIN 62  //Text Begin X-as
#define TBY_WIN 141
#define TBX_WIN_shadow 60
#define TBY_WIN_shadow 140

#define TBX_LOSE 62  //Text Begin X-as
#define TBY_LOSE 141
#define TBX_LOSE_shadow 60
#define TBY_LOSE_shadow 140

#define SMILEY_X 120
#define SMILEY_Y 250
#define SMILEY_R 50

#define SMILEY_EYE_L_X 100
#define SMILEY_EYE_L_Y 225
#define SMILEY_EYE_L_R 10

#define SMILEY_EYE_R_X 140
#define SMILEY_EYE_R_Y 225
#define SMILEY_EYE_R_R 10

#define SMILEY_MOUTH_X 120
#define SMILEY_MOUTH_Y 260
#define SMILEY_MOUTH_R 18

#define SMILEY_MOUTH_MOD_X 90
#define SMILEY_MOUTH_MOD_Y 240
#define SMILEY_MOUTH_MOD_W 50
#define SMILEY_MOUTH_MOD_H 20

#define SMILEY_DROP1_X 150
#define SMILEY_DROP1_Y 235
#define SMILEY_DROP1_R 5

#define SMILEY_DROP2_X 150
#define SMILEY_DROP2_Y 235
#define SMILEY_DROP2_R 5

class FinalScreen {
    public:
        void WinScreen(Adafruit_ILI9341 *tft);
        void LoseScreen(Adafruit_ILI9341 *tft);
    private:
};

#endif