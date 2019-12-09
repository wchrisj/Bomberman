#include "Waitingpage.h"
#include "libraries/IRlib/IR.h"
#include "libraries/Nunchuk/nunchuk.h"



Waitingpage::Waitingpage(Adafruit_ILI9341 *tft) {
    tft->begin();
}

void Waitingpage::show(Adafruit_ILI9341 *tft){                               // Het maken van het wacht scherm
  tft->fillScreen(ILI9341_BLACK);
  tft->setCursor(00, 60);
  tft->setTextColor(ILI9341_WHITE);  tft->setTextSize(2);
  tft->print("gelukt");
  tft->setCursor(20, 100);
  tft->setTextColor(ILI9341_BLUE);  tft->setTextSize(3);
  tft->print("wachten op");
  tft->setCursor(70, 140);
  tft->setTextColor(ILI9341_RED);  tft->setTextSize(2);
  tft->print("Tegenstander");

}