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
#include "Game.h"
#include "Waitingpage.h"

volatile char F_readNunchuk = 0;
volatile int Optellen = 0;
bool gestart = false;
NunchukInput* nunchuk = NunchukInput::getInstance();
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

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
	IR ir = IR(2, 38);
	ir.enableReceiver();			
	
	Homepage homepage = Homepage(&tft); //maak homepage object
	Waitingpage waitingpage = Waitingpage(&tft);
	
	homepage.HomepageText(&tft);
	gameTimerInit();
	LCD lcd = LCD(&tft);

	sei();
	status_t status;
	status = notReady;

	while (1)
	{	
		if (F_readNunchuk == 1){ 	// if statement om te zien of er een Nunchuk is
			F_readNunchuk == 0;		// Nunchuk status terugzetten naar 0
			nunchuk->nunchuk_get();	// waarde van de nunchuk lezen
		} else {
			continue;
		}

		if (status == notReady){
			if (nunchuk->status.Z == 1){
				if(!gestart) {
					gestart = true;
					continue;
				}
				Optellen++;
				status = waiting; 
				waitingpage.show(&tft);	// maak het wachtscherm
				ir.send(IDENTIFIER_START, 0, 0);	// beginnen met het zenden van Infrarood
				ir.enableReceiver();	// IR ontvanger aanzetten
			}else {
				continue;
			}
		}
		if(status == waiting){
			if(ir.decode()){
				if(ir.results.identifier == IDENTIFIER_START){
					status = playing;
				}
			} else{ 
				_delay_ms(500); //DIT MOET EEN GAME TICK WORDEN
				ir.send(IDENTIFIER_START,0,0);
				ir.enableReceiver();
				continue;
			}
		}
		if(status == playing){
			lcd.drawWall(&tft, 3, 4);	//Maak de muur			##
			lcd.drawMap(&tft);			//Maak de map			## Dit moet het echte spel gaan worden
			lcd.statusBar(&tft);			//Maak de status bar 	##
		} else {
			continue;
		}

	//	lcd.drawWall(tft, 3, 4);	//Maak de muur			##
	//	lcd.drawMap(tft);			//Maak de map			## Dit moet het echte spel gaan worden
	//	lcd.statusBar(tft);			//Maak de status bar 	##

		if(ir.decode()) {
			Serial.print(ir.results.data, HEX);
		}
		_delay_ms(100);
	}
	return(0);
}


	
