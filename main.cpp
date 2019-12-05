#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"
#include <HardwareSerial.h>
#include "libraries/Nunchuk/nunchuk.h"
#include "libraries/IRlib/IR.h"
#include <SPI.h> // moet nog iets op worden gevonden dat deze kan worden geinclude in LCD.h
#include "LCD.h"
#include "Homepage.h"

volatile char F_readNunchuk = 0;
volatile int Optellen = 0;
bool gestart = false;
NunchukInput* nunchuk = NunchukInput::getInstance();

void gameTimerInit() {
	TCCR1A = 0; // set entire TCCR1A register to 0
	TCCR1B = 0; // same for TCCR1B
	TCNT1 = 0; // initialize counter value to 0
	// set compare match register for 500 Hz increments
	OCR1A = 31999; // = 16000000 / (1 * 500) - 1 (must be <65536) 500hz 2ms
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS12, CS11 and CS10 bits for 1 prescaler
	TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
}

	ISR(TIMER1_COMPA_vect) { //Elke 2ms
		F_readNunchuk = 1;
			 
		
	}

int main(void)	//MAIN	
{
	sei();
	Serial.begin(115200);
	IR ir = IR(3, 38);
	ir.enableReceiver();			
	
	Homepage homepage = Homepage(); //maak homepage object
	
	homepage.HomepageText();
	gameTimerInit();
	LCD lcd = LCD();
	

	sei();
	while (1)
	{	
/*		if (F_readNunchuk == 1){ // if statement om naar de game te gaan
			F_readNunchuk == 0;
			nunchuk->nunchuk_get();
		} else {
			continue;
		}

		if ( nunchuk->status.Z == 1 ){
			if(!gestart) {
				gestart = true;
				continue;
			}
			Optellen++;
			gestart = true;
	 	}else {
			continue;
		}
*/
		lcd.drawWall(3, 4);			//HIER WERKT HET MAKEN VAN HET SCHERM NIET
		lcd.drawMap();				//############
		lcd.statusBar();			//############

		ir.send(0b11, 0x11, 8);
		ir.enableReceiver();
		if(ir.decode()) {
			Serial.print(ir.results.data, HEX);
		}
		_delay_ms(100);
	}
	return(0);
}


	
