#include "Waitingpage.h"
#include "LCD.h"
#include "libraries/IRlib/IR.h"
#include "libraries/Nunchuk/nunchuk.h"


void Waitingpage::show(Adafruit_ILI9341 *tft){                    // Het maken van het wacht scherm
  tft->fillScreen(ILI9341_BLACK);                                 // maak scherm zwart

  tft->setCursor(WP_CURSOR_X60, WP_CURSOR_Y10);                   // zet cursor op x60 en y10
  tft->setTextColor(ILI9341_BLUE);  tft->setTextSize(TEXTSIZE2);  // tekst grote en kleur kiezen
  tft->print("Bomberman");                                        // tekst printen
  tft->setCursor(WP_CURSOR_X10, WP_CURSOR_Y60);                   // zet cursor op x10 en y60
  tft->setTextColor(ILI9341_RED);  tft->setTextSize(TEXTSIZE3);   // tekst grote en kleur kiezen
  tft->print("wachten op:");                                      // tekst printen
  tft->setCursor(WP_CURSOR_X10, WP_CURSOR_Y100);                  // zet cursor op x10 en y100
  tft->setTextColor(ILI9341_WHITE);  tft->setTextSize(TEXTSIZE2); // tekst grote en kleur kiezen
  tft->print("Tegenstander");                                     // tekst printen
}

void Waitingpage::cycle(Adafruit_ILI9341 *tft, uint8_t *cycle_staps){ // Het maken van de wacht punten
  uint8_t i = 0;
    tft->setCursor(*cycle_staps * WP_DOT2DOT_SIZE, WP_CURSOR_Y120);       // zet cursor x(cycle_staps * dot2dot_size) en y120
    tft->setTextColor(ILI9341_YELLOW);  tft->setTextSize(TEXTSIZE4);      // tekst grote en kleur kiezen
    tft->print(".");                                                      // print tekst
  if(*cycle_staps){                                                     // als cycle_staps géén 0 is dan,

    tft->setCursor((*cycle_staps-1) * WP_DOT2DOT_SIZE, WP_CURSOR_Y120); //
      
  } else{
    tft->setCursor((MAXDOTS-1) * WP_DOT2DOT_SIZE, WP_CURSOR_Y120);      // anders,
  }
  tft->setTextColor(ILI9341_BLACK);  tft->setTextSize(TEXTSIZE4);       // verwijder het voorgande gele puntje     
  tft->print(".");                                                      // maak punt zwart zoals de achtergrond
}