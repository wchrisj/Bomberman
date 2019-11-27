#include "LCD.h"



Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

LCD::LCD() {
    tft.begin();
}

void LCD::drawWall(uint8_t x, uint8_t y) {
    tft.drawRect(x * blok, y * blok, blok, blok, ILI9341_WHITE);
    tft.drawRect(x * blok + 1, y * blok + 1, blok - 3, blok - 3, ILI9341_WHITE);
    tft.fillRect(x * blok + 2, y * blok + 2, blok - 4, blok - 4, ILI9341_LIGHTGREY);
    tft.drawPixel(x * blok, y * blok, ILI9341_BLACK);
    tft.drawPixel(x * blok + 1, y * blok, ILI9341_LIGHTGREY);
    tft.drawPixel(x * blok, y * blok + 1, ILI9341_LIGHTGREY);
    //objects[y * 15 + x] = 1; // zet getekende muur in objecten array
}

void LCD::drawMap(){
    tft.fillScreen(ILI9341_BLACK);
  // teken verticaal links                        // x─┐
  for (int i = 0; i < (lengte / blok) - 1; i++){  // x │
    drawWall(0, i);                                   // x─┘
  }
  
  //teken horizontaal boven                       // xxx
  for (int i = 0; i < (breedte / blok); i++){     // │ │
    drawWall(i, 0);                                   // └─┘
  }
  
  // teken horizontaal onder                      // ┌─┐
  for (int i = 0; i < (breedte / blok); i++){     // │ │
    drawWall(i, (lengte / blok) - 2);                 // xxx
  }
  
  // teken rechts verticaal                       // ┌─x
  for (int i = 0; i < (lengte / blok) - 1; i++){  // │ x
    drawWall((breedte / blok) - 1, i);                // └─x
  }

  // teken de losse blokken in het midden
  for (int i = 2; i < (lengte / blok) - 2; i += 2) {
    for (int k = 2; k < (breedte / blok) - 2; k += 2) {
      drawWall(k, i); // teken muur
    }
  }
  tft.setCursor(0, lengte - blok +2);
  tft.setTextColor(ILI9341_YELLOW);  tft.setTextSize(2);
  tft.print("P1");
  tft.setTextColor(ILI9341_BLUE);
  tft.print(" P2");
}