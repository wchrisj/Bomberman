#include "Homepage.h"
#include "libraries/IRlib/IR.h"
#include "libraries/Nunchuk/nunchuk.h"

void Homepage::HomepageText(Adafruit_ILI9341 *tft){               // Het maken van het Start scherm
  tft->fillScreen(ILI9341_BLACK);                                 // maak scherm zwart

  tft->setCursor(HP_CURSOR_X60, HP_CURSOR_Y10);                   // zet cursor op x60 en y10
  tft->setTextColor(ILI9341_BLUE);  tft->setTextSize(TEXTSIZE2);  // tekst grote en kleur kiezen
  tft->print("Bomberman");                                        // tekst printen
  tft->setCursor(HP_CURSOR_X10, HP_CURSOR_Y60);                   // zet cursor op x10 en y60
  tft->setTextColor(ILI9341_RED);  tft->setTextSize(TEXTSIZE4);   // tekst grote en kleur kiezen
  tft->print("Ready?");                                           // tekst printen
  tft->setCursor(HP_CURSOR_X10, HP_CURSOR_Y100);                  // zet cursor op x10 en y100
  tft->setTextColor(ILI9341_WHITE);  tft->setTextSize(TEXTSIZE2); // tekst grote en kleur kiezen
  tft->print("Gebruik de 'C' knop op de controller om te beginnen"); // tekst printen
}
