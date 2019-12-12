#ifndef Homepage_H
#define Homepage_H

#include <avr/io.h>  //zou mss mooi zijn als die uiteindelijk 1 keer word geinclude in de main
#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"
#include "libraries/Nunchuk/nunchuk.h"
#include "LCD.h"

#define HP_CURSOR_X10 10
#define HP_CURSOR_X60 60

#define HP_CURSOR_Y10 10
#define HP_CURSOR_Y60 60
#define HP_CURSOR_Y100 100

class Homepage {
    public:
        Homepage(Adafruit_ILI9341 *tft);
        void HomepageText(Adafruit_ILI9341 *tft);
    private:
};



#endif