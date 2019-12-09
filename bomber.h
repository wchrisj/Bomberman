#ifndef BOMBER.H
#define BOMBER.H
#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"
#include "libraries/Game_logic/character.h"
#include "libraries/Game_logic/bomb.h"
#include "libraries/Nunchuk/nunchuk.h"
#include "LCD.h"

#define TFT_CS 10 // TFT select pin
#define TFT_DC  9 // TFT display/command pin

extern Adafruit_ILI9341 tft;
extern Character localCharacter;
extern Character externCharacter;
// extern Bomb bombs[1];
extern NunchukInput* nunchuk;

#endif