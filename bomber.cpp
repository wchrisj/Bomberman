#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Adafruit_GFX.h"	// Core graphics library
#include "Adafruit_ILI9341.h"	// Hardware-specific library

#include "bomber.h"

void gameTimerInit();

//F = Flag
//C = Counter
volatile char F_readNunchuk = 0;
volatile char C_charMove = 0;
volatile char C_bomb = 0;

//Singleton design pattern
//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
NunchukInput* nunchuk = NunchukInput::getInstance();
Character* character = Character::getInstance();
Bomb* bomb = Bomb::getInstance();


ISR(TIMER1_COMPA_vect) { //Elke 2ms
	F_readNunchuk = 1;
	C_charMove++;
	if(bomb->exists) {
		C_bomb++;
	}
	if(C_charMove == 50) { //100ms
		C_charMove = 0;
		if (nunchuk->status.UP == 1) {
			character->move(Character::UP);
			Serial.println("UP");
		}
		else if (nunchuk->status.RIGHT == 1) {
			character->move(Character::RIGHT);
			Serial.println("RIGHT");
		}
		else if (nunchuk->status.DOWN == 1) {
			character->move(Character::DOWN);
			Serial.println("DOWN");
		}
		else if (nunchuk->status.LEFT == 1) {
			character->move(Character::LEFT);
			Serial.println("LEFT");
		}
		if (nunchuk->status.Z == 1) {
			bomb->placeBomb();
		}
	}

	if (C_bomb == 2000) { //4seconden
		C_bomb = 0;
		bomb->explodeBomb();
	}
}

int main (void)
{
	sei();
	_delay_ms(50);
	Serial.begin(9600);
	gameTimerInit();
	tft.begin();
	tft.fillScreen(ILI9341_BLACK);	

	character->init(0, 0, 20, 20, ILI9341_YELLOW);

	while (1)
	{
		//Wire van nunchuk gebruikt een ISR. ISR in ISR mag niet.
		//Vandaar een handmatige flag zodat het wel kan.
		if (F_readNunchuk == 1) {
			F_readNunchuk = 0;
			nunchuk->nunchuk_get();
		}
	}

	return(0);
}

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