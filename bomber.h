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

extern Adafruit_ILI9341 tft;
extern Character localCharacter;
extern Character externCharacter;
extern NunchukInput* nunchuk;
extern Map mapGenerator;

#endif