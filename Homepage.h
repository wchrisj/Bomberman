#ifndef Homepage_H
#define Homepage_H

#include <avr/io.h>  //zou mss mooi zijn als die uiteindelijk 1 keer word geinclude in de main
#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"
#include "libraries/Nunchuk/nunchuk.h"


#define TFT_DC 9
#define TFT_CS 10

#define BLOCK_SIZE 16
#define LENGTH 320
#define WIDTH 240


class Homepage {
    public:
        Homepage();
        void HomepageText();
        //void StartSending();
        void gameTimerInit();
        void HomepageText2();
    private:
};



#endif