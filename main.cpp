#include <avr/interrupt.h>
#include <util/delay.h>
#include <HardwareSerial.h>
#include "libraries/Nunchuk/nunchuk.h"
#include "libraries/IRlib/IR.h"
#include <SPI.h> // moet nog iets op worden gevonden dat deze kan worden geinclude in LCD.h
#include "LCD.h"

NunchukInput* nunchuk = NunchukInput::getInstance();

int main(void)
{
	sei();
	Serial.begin(115200);
  
   LCD lcd = LCD();
   lcd.drawWall(3, 4);
   lcd.drawMap();
   lcd.statusBar();
  
	IR ir = IR(3, 38);
	ir.enableReceiver();
	while (1)
	{
		nunchuk->nunchuk_get();
		ir.send(0b01, 0x00, 8);
		ir.enableReceiver();
		if(ir.decode()) {
			Serial.print(ir.results.data, HEX);
		}
		_delay_ms(100);
	}
}
