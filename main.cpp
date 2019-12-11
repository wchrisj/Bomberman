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
bool gestart = false;
uint8_t cycle_staps = 0;
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
	sei();					// aanzetten interupts
	Serial.begin(115200);	// Baudrate instellen
	//nunchuk->nunchuk_get();
	
	
	
	IR ir = IR(2, 56);
	ir.enableReceiver();				// IR ontvanger aanzetten				
	
	Homepage homepage = Homepage(&tft); 			// maak homepage object aan
	Waitingpage waitingpage = Waitingpage(&tft); 	// maak waitingpage object aan
	
	homepage.HomepageText(&tft); 	// maak homepage scherm (1e scherm, beginscherm)
	gameTimerInit();				// run game timer
	LCD lcd = LCD(&tft);			// maak lcd object aan

	status_t status;
	status = notReady;	// status beginnen met notReady
	uint8_t identifierReceived = 0;

while (1){
	_delay_ms(15);				//delayen.. hoger om IR signaal te kunnen uitlezen ###########################
	if( ir.decode()){			// IR uitlezen
		identifierReceived = 1;	// identifierReceived aanzetten
		ir.resumeReceiver();	// gaat volgende signaal lezen
	}
		
		if (F_readNunchuk == 1){ 	// if statement om te zien of er een Nunchuk is
			F_readNunchuk == 0;		// Nunchuk status terugzetten naar 0
			nunchuk->nunchuk_get();	// waarde van de nunchuk lezen
		} else {
			continue;
		}

		if (status == notReady){					// als status is notReady, dan
			if (nunchuk->status.Z == 1){			// kijken of de Z is ingedrukt met de Nunchuk
				if(!gestart) {						// als niet gestart, dan
					gestart = true;					// zet gestart op true
					continue;
				}
				status = waiting; 					// zet status op waiting
				waitingpage.show(&tft);				// maak het wachtscherm
			}else {
				continue;
			}
		}
		if(status == waiting){									// Als de status waiting is, dan
		Serial.println("Waiting");
			if(identifierReceived){									// De ontvangen data decoderen zodat er iets mee gedaan kan worden
				if(ir.results.identifier == IDENTIFIER_START){ 	// Als het IR signaal een start identifier stuurt
				Serial.println("IDENTIFIER");
					status = playing;							// dan, Zet status om naar playing
					Serial.println("MAP MAKEN");
					lcd.drawWall(&tft, 3, 4);					//Maak de muur			##
					lcd.drawMap(&tft);							//Maak de map			## Dit moet het echte spel gaan worden
					lcd.statusBar(&tft);						//Maak de status bar 	##
				}
			} else{ 
				_delay_ms(100); //DIT MOET EEN GAME TICK WORDEN #################################################
				cycle_staps++;							// cycle_staps verhogen met +1
				if (cycle_staps == MAXDOTS){			// Als cycle_staps de maximale aantal punten heeft geprint 
					cycle_staps = 0;					// dan, cycle_staps resetten naar 0
				}
				waitingpage.cycle(&tft, &cycle_staps);	// functie printen van dots
				ir.send(IDENTIFIER_START,0,0);			// Verzend IR signaal met alleen een identifier

				ir.enableReceiver();					// Zet de IR-ontvanger aan
				continue;
			}
		}
		if(status == playing){
			//## hier moet het "SPEL / GAMELOOP"
		}
	}
	return(0);
}


	
