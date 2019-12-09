#ifndef Waitingpage_H
#define Waitingpage_H

#include <avr/io.h>  //zou mss mooi zijn als die uiteindelijk 1 keer word geinclude in de main
#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"
#include "libraries/Nunchuk/nunchuk.h"


class Waitingpage {
    public:
        Waitingpage(Adafruit_ILI9341 *tft);
        void show(Adafruit_ILI9341 *tft);  
    private:
};




#endif