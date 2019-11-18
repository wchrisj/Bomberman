/*
 * Main.cpp
 *
 * Created: 13/11/2019 11:35:58
 * Author : sbrol
 */

#include <avr/interrupt.h>
#include <util/delay.h>
#include "nunchuk.h"

void setLed();
NunchukInput NI;

int main(void)
{
	sei();
	DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	while (1)
	{
		NI.nunchuk_get();
		setLed();
		_delay_ms(10);
	}
}

//VOOR DEBUGGING
void setLed() {
	if (NI.status.RIGHT == 0 && NI.status.LEFT == 0) {
		PORTD &= ~((1 << PORTD5) | (1 << PORTD7));
	}
	else if (NI.status.RIGHT == 1) {
		PORTD |= (1 << PORTD7);
		PORTD &= ~(1 << PORTD5);
	}
	else if (NI.status.LEFT == 1) {
		PORTD |= (1 << PORTD5);
		PORTD &= ~(1 << PORTD7);
	}

	if (NI.status.UP == 0 && NI.status.DOWN == 0) {
		PORTD &= ~((1 << PORTD4) | (1 << PORTD6));
	}
	else if (NI.status.UP == 1) {
		PORTD |= (1 << PORTD4);
		PORTD &= ~(1 << PORTD6);
	}
	else if (NI.status.DOWN == 1) {
		PORTD |= (1 << PORTD6);
		PORTD &= ~(1 << PORTD4);
	}

	if (NI.status.Z == 0 && NI.status.C == 0) {
		PORTD &= ~((1 << PORTD2) | (1 << PORTD3));
	}
	else if (NI.status.Z == 1 && NI.status.C == 1) {
		PORTD |= (1 << PORTD2) | (1 << PORTD3);
	}
	else if (NI.status.C == 1) {
		PORTD |= (1 << PORTD2);
		PORTD &= ~(1 << PORTD3);
	}
	else if (NI.status.Z == 1) {
		PORTD |= (1 << PORTD3);
		PORTD &= ~(1 << PORTD2);
	}
}

