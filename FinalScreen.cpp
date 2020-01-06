#include "FinalScreen.h"
#include "LCD.h"
#include "Waitingpage.h"
#include "Homepage.h"
#include "libraries/IRlib/IR.h"
#include "libraries/Nunchuk/nunchuk.h"

void FinalScreen::WinScreen(Adafruit_ILI9341 *tft){               // Het maken van het Start scherm
  tft->fillScreen(ILI9341_BLACK);                                 // maak scherm zwart

  tft->setCursor(TBX_YOU_SHADOW, TBY_YOU_SHADOW);                   
  tft->setTextColor(ILI9341_ORANGE);  tft->setTextSize(TEXTSIZE7);  // tekst grote en kleur kiezen
  tft->print("YOU");                                        // tekst printen
  tft->setCursor(TBX_YOU, TBY_YOU);                                         
  tft->setTextColor(ILI9341_YELLOW);  tft->setTextSize(TEXTSIZE7);   // tekst grote en kleur kiezen
  tft->print("YOU");   

  tft->setCursor(TBX_WIN_SHADOW, TBY_WIN_SHADOW);                  
  tft->setTextColor(ILI9341_YELLOW);  tft->setTextSize(TEXTSIZE6); // tekst grote en kleur kiezen
  tft->print("WIN!"); // tekst printen
  tft->setCursor(TBX_WIN, TBY_WIN);                  
  tft->setTextColor(ILI9341_GREEN);  tft->setTextSize(TEXTSIZE6); // tekst grote en kleur kiezen
  tft->print("WIN!"); // tekst printen

  tft->fillCircle(SMILEY_X, SMILEY_Y, SMILEY_R, ILI9341_YELLOW); // smiley geel
  tft->fillCircle(SMILEY_EYE_L_X, SMILEY_EYE_L_Y, SMILEY_EYE_L_R, ILI9341_BLACK); //oog links
  tft->fillCircle(SMILEY_EYE_R_X, SMILEY_EYE_R_Y, SMILEY_EYE_R_R, ILI9341_BLACK); //oog rechts
  tft->fillCircle(SMILEY_MOUTH_X, SMILEY_MOUTH_Y, SMILEY_MOUTH_R, ILI9341_BLACK);// mond cirkel
  tft->fillRect(SMILEY_MOUTH_MOD_X, SMILEY_MOUTH_MOD_Y, SMILEY_MOUTH_MOD_W, SMILEY_MOUTH_MOD_H, ILI9341_YELLOW); // mond aanpassing
}

void FinalScreen::LoseScreen(Adafruit_ILI9341 *tft){               // Het maken van het Start scherm
  tft->fillScreen(ILI9341_BLACK);                                 // maak scherm zwart

  tft->setCursor(TBX_YOU_SHADOW, TBY_YOU_SHADOW);                  
  tft->setTextColor(ILI9341_ORANGE);  tft->setTextSize(TEXTSIZE7);  // tekst grote en kleur kiezen
  tft->print("YOU");                                        // tekst printen
  tft->setCursor(TBX_YOU, TBY_YOU);                                       
  tft->setTextColor(ILI9341_YELLOW);  tft->setTextSize(TEXTSIZE7);   // tekst grote en kleur kiezen
  tft->print("YOU"); 

  tft->setCursor(TBX_LOSE_SHADOW, TBY_LOSE_SHADOW);                  // zet cursor op x10 en y100
  tft->setTextColor(ILI9341_ORANGE);  tft->setTextSize(TEXTSIZE6); // tekst grote en kleur kiezen
  tft->print("LOSE"); // tekst printen
  tft->setCursor(TBX_LOSE, TBY_LOSE);                  // zet cursor op x10 en y100
  tft->setTextColor(ILI9341_RED);  tft->setTextSize(TEXTSIZE6); // tekst grote en kleur kiezen
  tft->print("LOSE"); // tekst printen

  tft->fillCircle(SMILEY_X, SMILEY_Y, SMILEY_R, ILI9341_YELLOW); // smiley geel
  tft->fillCircle(SMILEY_EYE_L_X, SMILEY_EYE_L_Y, SMILEY_EYE_L_R, ILI9341_BLACK); //oog links
  tft->fillCircle(SMILEY_EYE_R_X, SMILEY_EYE_R_Y, SMILEY_EYE_R_R, ILI9341_BLACK); //oog rechts
  tft->fillCircle(SMILEY_MOUTH_X, SMILEY_MOUTH_Y, SMILEY_MOUTH_R, ILI9341_BLACK);// mond cirkel
  tft->fillRect(SMILEY_MOUTH_MOD_X, SMILEY_MOUTH_MOD_Y, SMILEY_MOUTH_MOD_W, SMILEY_MOUTH_MOD_H, ILI9341_YELLOW); // mond aanpassing
  tft->fillCircle(SMILEY_DROP1_X, SMILEY_DROP1_Y, SMILEY_DROP1_R, ILI9341_BLUE); //druppel
}