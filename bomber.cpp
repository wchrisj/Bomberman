#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Adafruit_GFX.h"	// Core graphics library
#include "Adafruit_ILI9341.h"	// Hardware-specific library

#include "bomber.h"

void gameTimerInit();
void draw();

//F = Flag
//C = Counter
volatile char F_readNunchuk = 0;
volatile char C_charMove = 0;
volatile int C_bombs[1];

//Singleton design pattern
//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
NunchukInput* nunchuk = NunchukInput::getInstance();
Character* character = Character::getInstance();
Bomb bombs[1];

ISR(TIMER1_COMPA_vect) { //Elke 2ms
	F_readNunchuk = 1;
	C_charMove++;
	if(bombs[0].exists) {
		C_bombs[0]++;
	}
	if(bombs[1].exists) {
		C_bombs[1]++;
	}
	if(C_charMove == 100) { //200ms
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
			if(!bombs[0].exists) {
				bombs[0].placeBomb();
			}
		}
		draw();
	}

	if (C_bombs[0] == 6000) { //12seconden
		C_bombs[0] = 0;
		bombs[0].explodeBomb();
	}
	if (C_bombs[1] == 2000) {
		C_bombs[1] = 0;
		bombs[1].explodeBomb();
	}
}

int main (void)
{
	sei();
	_delay_ms(50);
	Serial.begin(9600);
	tft.begin();
	tft.fillScreen(ILI9341_BLACK);	
	character->init(0, 0, 20, 20, ILI9341_YELLOW);
	gameTimerInit();

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

void draw() {
	if((character->prevX != character->x) || (character->prevY != character->y)) {	
		tft.fillRect(character->prevY, character->prevX, character->height, character->width, ILI9341_BLACK);
	}
	tft.fillRect(character->y, character->x, character->height, character->width, ILI9341_YELLOW);
	if(bombs[0].exists == true) {
		tft.fillRect(bombs[0].bombY, bombs[0].bombX, character->height, character->width, ILI9341_RED);
	} else {
		tft.fillRect(bombs[0].bombY, bombs[0].bombX, character->height, character->width, ILI9341_BLACK);
	}
}