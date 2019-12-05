#include "LCD.h"
#include "bomber.h"

LCD::LCD() {
    tft.begin();
    tft.fillScreen(BG_COLOR);
}

// dit zijn geen magic numbers
void LCD::drawWall(uint8_t x, uint8_t y) {
    tft.drawRect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, ILI9341_WHITE);
    tft.drawRect(x * BLOCK_SIZE + 1, y * BLOCK_SIZE + 1, BLOCK_SIZE - 3, BLOCK_SIZE - 3, ILI9341_WHITE);
    tft.fillRect(x * BLOCK_SIZE + 2, y * BLOCK_SIZE + 2, BLOCK_SIZE - 4, BLOCK_SIZE - 4, ILI9341_LIGHTGREY);
    tft.drawPixel(x * BLOCK_SIZE, y * BLOCK_SIZE, ILI9341_DARKGREY);
    tft.drawPixel(x * BLOCK_SIZE + 1, y * BLOCK_SIZE, ILI9341_LIGHTGREY);
    tft.drawPixel(x * BLOCK_SIZE, y * BLOCK_SIZE + 1, ILI9341_LIGHTGREY);
    // objects[y * 15 + x] = 1; // zet getekende muur in objecten array (maar array is hier nog niet aangemaakt)
    // dit moet worden gedaan na merge met Test/Master
}

void LCD::drawBaseMap(){
  // teken verticaal links                                  // x─┐
  for (uint8_t i = 0; i < (LENGTH / BLOCK_SIZE) - 1; i++){  // x │
    drawWall(0, i);                                         // x─┘
  }
  
  //teken horizontaal boven                                 // xxx
  for (uint8_t i = 0; i < (WIDTH / BLOCK_SIZE); i++){       // │ │
    drawWall(i, 0);                                         // └─┘
  }
  
  // teken horizontaal onder                                // ┌─┐
  for (uint8_t i = 0; i < (WIDTH / BLOCK_SIZE); i++){       // │ │
    drawWall(i, (LENGTH / BLOCK_SIZE) - 2);                 // xxx
  }
  
  // teken rechts verticaal                                 // ┌─x
  for (uint8_t i = 0; i < (LENGTH / BLOCK_SIZE) - 1; i++){  // │ x
    drawWall((WIDTH / BLOCK_SIZE) - 1, i);                  // └─x
  }

  // teken de losse muur stukken in het midden
  for (uint8_t i = 2; i < (LENGTH / BLOCK_SIZE) - 2; i += 2) {
    for (uint8_t k = 2; k < (WIDTH / BLOCK_SIZE) - 2; k += 2) {
      drawWall(k, i); // teken muur
    }
  }
}

// deze functie zet "P1" en "P2" op de statusbar
// hier uiteindelijk ook misschien de ❤️ tekenen
void LCD::drawStatusBar(){
  tft.fillRect(0, LENGTH - BLOCK_SIZE, WIDTH, BLOCK_SIZE, ILI9341_BLACK);
  tft.setCursor(0, LENGTH - BLOCK_SIZE +2);
  tft.setTextColor(P1_COLOR);  tft.setTextSize(2);
  tft.print("P1");
  tft.setTextColor(P2_COLOR);
  tft.setCursor(BLOCK_SIZE * P2_STATUS_LOCATION, LENGTH - BLOCK_SIZE + 2);
  tft.print("P2");
}

// DEZE FUNCTIE MOET NOG VERDER WORDEN UITGEWERKT
// uint8_t p geeft de player aan
void LCD::updateLives(uint8_t p){
  tft.fillRect(BLOCK_SIZE * 2, LENGTH - BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, ILI9341_BLACK);
}