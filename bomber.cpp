#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Adafruit_GFX.h"	// Core graphics library
#include "Adafruit_ILI9341.h"	// Hardware-specific library

#include "libraries/Game_logic/character.h"
#include "libraries/Nunchuk/nunchuk.h"
#include "libraries/Game_logic/bomb.h"

void gameTimerInit();

Character character;
NunchukInput Nunchuk;

volatile char flag = 0;
volatile char stepper = 0;
volatile char bombTimer = 0;

ISR(TIMER1_COMPA_vect) {
	flag = 1;
	stepper++;
	bombTimer++;
	if(stepper == 50) { //100ms
		stepper = 0;
		if (Nunchuk.status.UP == 1) {
			character.move(Character::UP);
		}
		else if (Nunchuk.status.RIGHT == 1) {
			character.move(Character::RIGHT);
		}
		else if (Nunchuk.status.DOWN == 1) {
			character.move(Character::DOWN);
		}
		else if (Nunchuk.status.LEFT == 1) {
			character.move(Character::LEFT);
		}
		if (Nunchuk.status.Z == 1) {
			bomb.placeBomb();
		}
	}

	if (bombTimer == 2000) { //4seconds
		bomb.explodeBomb();
	}
}

int main (void)
{
	sei();
	_delay_ms(50);
	gameTimerInit();
	character.tft.begin();
	character.tft.fillScreen(ILI9341_BLACK);	

	character.init(0, 0, 20, 20, ILI9341_YELLOW);

	while (1)
	{
		if (flag == 1) {
			flag = 0;
			Nunchuk.nunchuk_get();
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