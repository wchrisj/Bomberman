#include "LCD.h"
#include "bomber.h"

LCD::LCD() {
    tft.begin();
    tft.fillScreen(BG_COLOR);
}

void LCD::drawAir(uint8_t x, uint8_t y){
  //tft.drawRect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, ILI9341_BLACK);
  tft.fillRect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BG_COLOR);
}

void LCD::drawWall(uint8_t x, uint8_t y) {
    tft.drawRect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, ILI9341_WHITE);
    tft.drawRect(x * BLOCK_SIZE + 1, y * BLOCK_SIZE + 1, BLOCK_SIZE - 3, BLOCK_SIZE - 3, ILI9341_WHITE);
    tft.fillRect(x * BLOCK_SIZE + 2, y * BLOCK_SIZE + 2, BLOCK_SIZE - 4, BLOCK_SIZE - 4, ILI9341_LIGHTGREY);
    tft.drawPixel(x * BLOCK_SIZE, y * BLOCK_SIZE, ILI9341_BLACK);
    tft.drawPixel(x * BLOCK_SIZE + 1, y * BLOCK_SIZE, ILI9341_LIGHTGREY);
    tft.drawPixel(x * BLOCK_SIZE, y * BLOCK_SIZE + 1, ILI9341_LIGHTGREY);
    //objects[y * 15 + x] = 1; // zet getekende muur in objecten array (maar array is hier nog niet aangemaakt)
}

void LCD::drawCrate(uint8_t x, uint16_t y){
  x = x * BLOCK_SIZE; // anders heel vaak "x * BLOK_SIZE" moeten doen, dit is leesbaarder (en mogenlijk sneller?)
  y = y * BLOCK_SIZE;
  // buiten randen
  tft.drawRect(x + 1, y + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2, CRATE_SHADOW);
  tft.drawRect(x + 1, y + 1, BLOCK_SIZE - 3, BLOCK_SIZE - 3, CRATE_HIGHLIGHT);
  tft.drawPixel(x + 1, y + 1, CRATE_SHADOW); // links boven hoekje
  tft.fillRect(x + 2, y + 2, BLOCK_SIZE - 4, BLOCK_SIZE - 4, CRATE_FILLING); // binnekant opvullen
  // binnen diepte
  tft.drawFastHLine(x + 3, y + 3, 9, CRATE_SHADOW);
  tft.drawFastVLine(x + 3, y + 4, 9, CRATE_SHADOW);
  tft.drawFastHLine(x + 4, y + 12, 9, CRATE_HIGHLIGHT);
  tft.drawFastVLine(x + 12, y + 3, 9, CRATE_HIGHLIGHT);
  // diagonaal
  tft.drawLine(x + 4, y + 6, x + 9, y + 11, CRATE_SHADOW);
  tft.drawLine(x + 6, y + 4, x + 11, y + 9, CRATE_HIGHLIGHT);
}

void LCD::drawBomb(uint8_t x, uint16_t y){
  x = x * BLOCK_SIZE;
  y = y * BLOCK_SIZE;
  tft.drawPixel(x + 10, y + 4, ILI9341_WHITE);
  tft.drawPixel(x + 13, y + 3, ILI9341_WHITE);
  tft.drawPixel(x + 14, y + 6, ILI9341_WHITE);
  tft.drawLine(x + 8, y + 11, x + 12, y + 5, 0xEFE4);
  tft.drawLine(x + 3, y + 11, x + 9, y + 8, ILI9341_RED);
  tft.drawLine(x + 3, y + 12, x + 9, y + 9, ILI9341_RED);
  tft.drawLine(x + 3, y + 13, x + 9, y + 10, ILI9341_RED);
  tft.drawLine(x + 3, y + 14, x + 9, y + 11, ILI9341_RED);
  tft.drawFastVLine(x + 2, y + 12, 2, 0xE000);
  tft.drawFastVLine(x + 10, y + 8, 3, 0xF904);
}

void LCD::drawExplosion(uint8_t x, uint16_t y, uint8_t i){
  x = x * BLOCK_SIZE;
  y = y * BLOCK_SIZE;
  if(i == 0){
    tft.fillRect(x, y, BLOCK_SIZE, BLOCK_SIZE, OUTER);
    tft.fillRect(x + 2, y + 2, 12, 12, MIDDLE);
    tft.fillRect(x, y + 3, BLOCK_SIZE, 10, MIDDLE);
    tft.fillRect(x + 3, y, 10, BLOCK_SIZE, MIDDLE);
    tft.fillRect(x + 4, y + 4, 8, 8, INNER);
    tft.fillRect(x, y + 5, BLOCK_SIZE, 6, INNER);
    tft.fillRect(x + 5, y, 6, BLOCK_SIZE, INNER);
    tft.fillRect(x + 3, y + 7, 10, 2, ILI9341_WHITE);
    tft.fillRect(x + 7, y + 3, 2, 10, ILI9341_WHITE);
    tft.fillRoundRect(x + 5, y + 5, 6, 6, 1, ILI9341_WHITE);
  }
  if(i == 1 || i == 2){
    tft.drawFastVLine(x + 4, y, BLOCK_SIZE, MIDDLE);
    tft.drawFastVLine(x + 11, y, BLOCK_SIZE, MIDDLE);
    tft.fillRect(x + 5, y, 6, BLOCK_SIZE, INNER);
    if(i == 1){
        tft.drawFastVLine(x + 3, y + 9, 7, MIDDLE);
        tft.drawFastVLine(x + 12, y + 9, 7, MIDDLE);
        tft.drawFastVLine(x + 2, y + 10, 3, OUTER);
        tft.drawFastVLine(x + 13, y + 10, 3, OUTER);
        tft.drawPixel(x + 1, y + 12, OUTER);
        tft.drawPixel(x + 14, y + 12, OUTER);
        tft.fillRect(x, y + 13, 3, 3, OUTER);
        tft.fillRect(x + 13, y + 13, 3, 3, OUTER);
    } else {
        tft.drawFastVLine(x + 3, y, 7, MIDDLE);
        tft.drawFastVLine(x + 12, y, 7, MIDDLE);
        tft.drawFastVLine(x + 2, y, 3, OUTER);
        tft.drawFastVLine(x + 13, y, 3, OUTER);
        tft.drawPixel(x + 1, y, OUTER);
        tft.drawPixel(x + 14, y, OUTER);
        tft.fillRect(x, y, 3, 3, OUTER);
        tft.fillRect(x + 13, y, 3, 3, OUTER);
    }
  }
  else if (i == 3 || i == 4){
    tft.drawFastHLine(x, y + 4, BLOCK_SIZE, MIDDLE);
    tft.drawFastHLine(x, y + 11, BLOCK_SIZE, MIDDLE);
    tft.fillRect(x, y + 5, BLOCK_SIZE, 6, INNER);
    if(i == 3){
        tft.drawFastHLine(x + 9, y + 3, 7, MIDDLE);
        tft.drawFastHLine(x + 9, y + 12, 7, MIDDLE);
        tft.drawFastHLine(x + 10, y + 2, 3, OUTER);
        tft.drawFastHLine(x + 10, y + 13, 3, OUTER);
        tft.drawPixel(x + 12, y + 1, OUTER);
        tft.drawPixel(x + 12, y + 14, OUTER);
        tft.fillRect(x + 13, y, 3, 3, OUTER);
        tft.fillRect(x + 13, y + 13, 3, 3, OUTER);
    } else {
        tft.drawFastHLine(x, y + 3, 7, MIDDLE);
        tft.drawFastHLine(x, y + 12, 7, MIDDLE);
        tft.drawFastHLine(x + 3, y + 2, 3, OUTER);
        tft.drawFastHLine(x + 3, y + 13, 3, OUTER);
        tft.drawPixel(x + 3, y + 1, OUTER);
        tft.drawPixel(x + 3, y + 14, OUTER);
        tft.fillRect(x, y, 3, 3, OUTER);
        tft.fillRect(x, y + 13, 3, 3, OUTER);
    }
  }
  else if (i == 5){
    tft.fillRect(x + 4, y + 11, 8, 5, MIDDLE);
    tft.fillRect(x + 5, y + 12, 6, 4, INNER);
    tft.drawFastHLine(x + 6, y + 11, 4, INNER);
    tft.drawFastHLine(x + 5, y + 10, 6, MIDDLE);
    tft.drawFastHLine(x + 6, y + 9, 4, MIDDLE);
  }
  else if (i == 6){
    tft.fillRect(x + 4, y, 8, 5, MIDDLE);
    tft.fillRect(x + 5, y, 6, 4, INNER);
    tft.drawFastHLine(x + 6, y + 4, 4, INNER);
    tft.drawFastHLine(x + 5, y + 5, 6, MIDDLE);
    tft.drawFastHLine(x + 6, y + 6, 4, MIDDLE);
  }
  else if (i == 7){
    tft.fillRect(x + 11, y + 4, 5, 8, MIDDLE);
    tft.fillRect(x + 12, y + 5, 4, 6, INNER);
    tft.drawFastVLine(x + 11, y + 6, 4, INNER);
    tft.drawFastVLine(x + 10, y + 5, 6, MIDDLE);
    tft.drawFastVLine(x + 9, y + 6, 4, MIDDLE);
  }
  else if (i == 8){
    tft.fillRect(x, y + 4, 5, 8, MIDDLE);
    tft.fillRect(x, y + 5, 4, 6, INNER);
    tft.drawFastVLine(x + 4, y + 6, 4, INNER);
    tft.drawFastVLine(x + 5, y + 5, 6, MIDDLE);
    tft.drawFastVLine(x + 6, y + 6, 4, MIDDLE);
  }
}

void LCD::drawMap(){
  for(short i = 0; i < MAP_SIZE; i++ ){
    if (mapGenerator.map[i] == TYPE_AIR) {
      //tft.fillRect((i % MAP_WIDTH)*16, ((i-(i%MAP_WIDTH))/MAP_WIDTH)*16, 16, 16, ILI9341_BLACK);
      //drawAir(i % MAP_WIDTH, (i-(i%MAP_WIDTH))/MAP_WIDTH); //deze is beetje gaar
    }
    else if (mapGenerator.map[i] == TYPE_WALL) {
      //tft.fillRect((i % MAP_WIDTH)*16, ((i-(i%MAP_WIDTH))/MAP_WIDTH)*16, 16, 16, ILI9341_LIGHTGREY);
      drawWall(i % MAP_WIDTH, (i-(i%MAP_WIDTH))/MAP_WIDTH);
    }
    else if (mapGenerator.map[i] == TYPE_CRATE) {
      //tft.fillRect((i % MAP_WIDTH)*16, ((i-(i%MAP_WIDTH))/MAP_WIDTH)*16, 16, 16, ILI9341_ORANGE);
      drawCrate(i % MAP_WIDTH, (i-(i%MAP_WIDTH))/MAP_WIDTH);
    }
  }
  //   tft.fillScreen(ILI9341_BLACK);
  // // teken verticaal links                              // x─┐
  // for (int i = 0; i < (LENGTH / BLOCK_SIZE) - 1; i++){  // x │
  //   drawWall(0, i);                                     // x─┘
  // }
  
  // //teken horizontaal boven                             // xxx
  // for (int i = 0; i < (WIDTH / BLOCK_SIZE); i++){       // │ │
  //   drawWall(i, 0);                                     // └─┘
  // }
  
  // // teken horizontaal onder                            // ┌─┐
  // for (int i = 0; i < (WIDTH / BLOCK_SIZE); i++){       // │ │
  //   drawWall(i, (LENGTH / BLOCK_SIZE) - 2);             // xxx
  // }
  
  // // teken rechts verticaal                             // ┌─x
  // for (int i = 0; i < (LENGTH / BLOCK_SIZE) - 1; i++){  // │ x
  //   drawWall((WIDTH / BLOCK_SIZE) - 1, i);              // └─x
  // }

  // // teken de losse BLOCK_SIZEken in het midden
  // for (int i = 2; i < (LENGTH / BLOCK_SIZE) - 2; i += 2) {
  //   for (int k = 2; k < (WIDTH / BLOCK_SIZE) - 2; k += 2) {
  //     drawWall(k, i); // teken muur
  //   }
  // }
}

void LCD::drawPlayer(uint8_t x, uint16_t y, uint16_t c){
  x = x * BLOCK_SIZE;
  y = y * BLOCK_SIZE;
  //uint16_t c = 0xF800;
  //uint16_t c = 0x0526;
  // hoofd
  tft.drawFastHLine(x + 5, y, 6, c);
  tft.drawFastHLine(x + 4, y + 1, 10, c);
  tft.fillRect(x + 5, y + 2, 7, 5, SKIN);
  tft.drawFastHLine(x + 4, y + 2, 3, BROWN);
  tft.drawPixel(x + 5, y + 3, BROWN);
  tft.drawFastHLine(x + 5, y + 2, 2, BROWN);
  tft.fillRect(x + 3, y + 3, 2, 3, BROWN);
  tft.drawFastVLine(x + 4, y + 3, 2, SKIN);
  tft.fillRect(x + 12, y + 3, 2, 2, SKIN);
  tft.drawPixel(x + 12, y + 6, SKIN);
  tft.drawFastHLine(x + 10, y + 5, 3, BG_COLOR);
  tft.drawPixel(x + 11, y + 4, BG_COLOR);
  tft.drawFastVLine(x + 10, y + 2, 2, BG_COLOR);
  // lichaam
  tft.drawFastHLine(x + 4, y + 7, 8, c);
  tft.drawFastHLine(x + 3, y + 8, 10, c);
  tft.drawFastHLine(x + 2, y + 9, 12, c);
  tft.fillRect(x + 2, y + 10, 3, 3, SKIN);  // hand
  tft.fillRect(x + 11, y + 10, 3, 3, SKIN); // hand
  tft.drawFastHLine(x + 4, y + 10, 8, c);
  tft.fillRect(x + 5, y + 10, 6, 3, PANTS);
  tft.drawFastHLine(x + 6, y + 10, 4, 0xFFE0); // goud
  tft.drawFastHLine(x + 7, y + 10, 2, PANTS);
  tft.drawFastVLine(x + 6, y + 7, 3, PANTS);
  tft.drawFastVLine(x + 9, y + 7, 3, PANTS);
  // benen
  tft.fillRect(x + 4, y + 12, 3,  2, PANTS);
  tft.fillRect(x + 9, y + 12, 3,  2, PANTS);
  tft.fillRect(x + 3, y + 14 , 3, 2, BROWN);
  tft.drawPixel(x + 2, y + 15, BROWN);
  tft.fillRect(x + 10, y + 14 , 3, 2, BROWN);
  tft.drawPixel(x + 13, y + 15, BROWN);
}

void LCD::statusBar(){
  tft.fillRect(0, LENGTH - BLOCK_SIZE, WIDTH, BLOCK_SIZE, ILI9341_BLACK);
  tft.setCursor(0, LENGTH - BLOCK_SIZE +2);
  tft.setTextColor(PLAYER_1);  tft.setTextSize(2);
  tft.print("P1");
  tft.setCursor(5 * BLOCK_SIZE, LENGTH - BLOCK_SIZE +2);
  tft.setTextColor(PLAYER_2);
  tft.print(" P2");
}