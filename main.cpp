#include <SPI.h>
#include "LCD.h"

int main(){
    LCD lcd = LCD();
    lcd.drawWall(3, 4);
    lcd.drawMap();
    return 1;
}