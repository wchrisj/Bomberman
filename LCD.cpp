#include "LCD.h"





LCD::LCD(Adafruit_ILI9341 *tft) {
    tft->begin();
}

void LCD::drawWall(Adafruit_ILI9341 *tft, uint8_t x, uint8_t y) {
    tft->drawRect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, ILI9341_WHITE);
    tft->drawRect(x * BLOCK_SIZE + 1, y * BLOCK_SIZE + 1, BLOCK_SIZE - 3, BLOCK_SIZE - 3, ILI9341_WHITE);
    tft->fillRect(x * BLOCK_SIZE + 2, y * BLOCK_SIZE + 2, BLOCK_SIZE - 4, BLOCK_SIZE - 4, ILI9341_LIGHTGREY);
    tft->drawPixel(x * BLOCK_SIZE, y * BLOCK_SIZE, ILI9341_BLACK);
    tft->drawPixel(x * BLOCK_SIZE + 1, y * BLOCK_SIZE, ILI9341_LIGHTGREY);
    tft->drawPixel(x * BLOCK_SIZE, y * BLOCK_SIZE + 1, ILI9341_LIGHTGREY);
    //objects[y * 15 + x] = 1; // zet getekende muur in objecten array (maar array is hier nog niet aangemaakt)
}

void LCD::drawMap(Adafruit_ILI9341 *tft){
    tft->fillScreen(ILI9341_BLACK);
  // teken verticaal links                              // x─┐
  for (int i = 0; i < (LENGTH / BLOCK_SIZE) - 1; i++){  // x │
    drawWall(tft, 0, i);                                // x─┘
  }
  
  //teken horizontaal boven                             // xxx
  for (int i = 0; i < (WIDTH / BLOCK_SIZE); i++){       // │ │
    drawWall(tft, i, 0);                                // └─┘
  }
  
  // teken horizontaal onder                            // ┌─┐
  for (int i = 0; i < (WIDTH / BLOCK_SIZE); i++){       // │ │
    drawWall(tft, i, (LENGTH / BLOCK_SIZE) - 2);        // xxx
  }
  
  // teken rechts verticaal                             // ┌─x
  for (int i = 0; i < (LENGTH / BLOCK_SIZE) - 1; i++){  // │ x
    drawWall(tft, (WIDTH / BLOCK_SIZE) - 1, i);        // └─x
  }

  // teken de losse BLOCK_SIZEken in het midden
  for (int i = 2; i < (LENGTH / BLOCK_SIZE) - 2; i += 2) {
    for (int k = 2; k < (WIDTH / BLOCK_SIZE) - 2; k += 2) {
      drawWall(tft, k, i); // teken muur
    }
  }
}

void LCD::statusBar(Adafruit_ILI9341 *tft){
  tft->setCursor(0, LENGTH - BLOCK_SIZE +2);
  tft->setTextColor(ILI9341_YELLOW);  tft->setTextSize(2);
  tft->print("P1");
  tft->setTextColor(ILI9341_BLUE);
  tft->print(" P2");
}