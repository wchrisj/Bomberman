#include "Homepage.h"
#include "libraries/IRlib/IR.h"
#include "libraries/Nunchuk/nunchuk.h"



Homepage::Homepage(Adafruit_ILI9341 *tft) {
    tft->begin();
}

void Homepage::HomepageText(Adafruit_ILI9341 *tft){                               // Het maken van het Start scherm
  tft->fillScreen(ILI9341_GREENYELLOW);
  tft->setCursor(00, 60);
  tft->setTextColor(ILI9341_WHITE);  tft->setTextSize(2);
  tft->print("Druk 'Z' in ");
  tft->setCursor(20, 100);
  tft->setTextColor(ILI9341_BLUE);  tft->setTextSize(3);
  tft->print("we bouwen");
  tft->setCursor(70, 140);
  tft->setTextColor(ILI9341_RED);  tft->setTextSize(3);
  tft->print("A.U.B");

}

void Homepage::HomepageText2(Adafruit_ILI9341 tft){
 tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(00, 60);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(2);
  tft.print("Druk 'Z' in ");
  tft.setCursor(20, 100);
  tft.setTextColor(ILI9341_BLUE);  tft.setTextSize(3);
  tft.print("Even geduld");
  tft.setCursor(70, 140);
  tft.setTextColor(ILI9341_RED);  tft.setTextSize(3);
  tft.print("A.U.B");

}
