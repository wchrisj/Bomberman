#ifndef BOMBER.H
#define BOMBER.H
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "libraries/Game_logic/character.h"
#include "libraries/Game_logic/bomb.h"
#include "libraries/Nunchuk/nunchuk.h"

#define TFT_CS 10 // TFT select pin
#define TFT_DC  9 // TFT display/command pin

extern Adafruit_ILI9341 tft;
extern Character* character;
extern Bomb* bomb;
extern NunchukInput* nunchuk;

#endif