#ifndef Waitingpage_H
#define Waitingpage_H

#include <avr/io.h>  //zou mss mooi zijn als die uiteindelijk 1 keer word geinclude in de main
#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"
#include "libraries/Nunchuk/nunchuk.h"

#define WP_CURSOR_X10 10
#define WP_CURSOR_X60 60

#define WP_CURSOR_Y10 10
#define WP_CURSOR_Y60 60
#define WP_CURSOR_Y100 100
#define WP_CURSOR_Y120 120

#define MAXDOTS 11
#define WP_DOT2DOT_SIZE 20

class Waitingpage {
    public:
        void show(Adafruit_ILI9341 *tft);  
        void cycle(Adafruit_ILI9341 *tft, uint8_t *cycle_staps);
    private:
};




#endif