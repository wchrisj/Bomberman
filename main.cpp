#include <SPI.h>
#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"

#define TFT_DC 9
#define TFT_CS 10

#define blok 16
#define lengte 320
#define breedte 240

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

int main(){
    tft.begin();
    tft.fillScreen(ILI9341_BLACK);
    return 1;
}