#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"
#include "bomber.h"

#define CHARACTER_MOVE 100
#define BOMB_EXPLODE 2000

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
Character localCharacter;
Character externCharacter;
//Bomb bombs[1];

ISR(TIMER1_COMPA_vect) { //Elke 2ms
	F_readNunchuk = 1;
	C_charMove++;
	if(localCharacter.bomb.exists) {
		C_bombs[0]++;
	}
	if(externCharacter.bomb.exists) {
		C_bombs[1]++;
	}
	if(C_charMove == CHARACTER_MOVE) { //200ms
		C_charMove = 0;
		if (nunchuk->status.UP == 1) {
			localCharacter.move(Character::UP);
			Serial.println("UP");
		}
		else if (nunchuk->status.RIGHT == 1) {
			localCharacter.move(Character::RIGHT);
			Serial.println("RIGHT");
		}
		else if (nunchuk->status.DOWN == 1) {
			localCharacter.move(Character::DOWN);
			Serial.println("DOWN");
		}
		else if (nunchuk->status.LEFT == 1) {
			localCharacter.move(Character::LEFT);
			Serial.println("LEFT");
		}
		if (nunchuk->status.Z == 1) {
			if(!localCharacter.bomb.exists) {
				localCharacter.bomb.placeBomb(localCharacter.x, localCharacter.y);
			}
		}
		draw();
	}

	if (C_bombs[0] == BOMB_EXPLODE) { //12seconden
		C_bombs[0] = 0;
		localCharacter.bomb.explodeBomb();
	}
	if (C_bombs[1] == BOMB_EXPLODE) {
		C_bombs[1] = 0;
		externCharacter.bomb.explodeBomb();
	}
}

int main (void)
{
	sei();
	_delay_ms(50);
	Serial.begin(9600);
	tft.begin();
	LCD lcd = LCD();
	lcd.drawMap();
	localCharacter.init(16, 16, ILI9341_YELLOW);
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
	if((localCharacter.prevX != localCharacter.x) || (localCharacter.prevY != localCharacter.y)) {	
		tft.fillRect(localCharacter.prevX, localCharacter.prevY, localCharacter.height, localCharacter.width, ILI9341_BLACK);
	}
	tft.fillRect(localCharacter.x, localCharacter.y, localCharacter.height, localCharacter.width, ILI9341_YELLOW);
	if(localCharacter.bomb.exists == true) {
		tft.fillRect(localCharacter.bomb.bombX, localCharacter.bomb.bombY, localCharacter.height, localCharacter.width, ILI9341_RED);
	} else {
		tft.fillRect(localCharacter.bomb.bombX, localCharacter.bomb.bombY, localCharacter.height, localCharacter.width, ILI9341_BLACK);
	}
}