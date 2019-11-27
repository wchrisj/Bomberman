/*
 * Main.cpp
 *
 * Created: 13/11/2019 11:35:58
 * Author : sbrol
 */

#include <avr/interrupt.h>
#include <util/delay.h>
#include "libraries/Nunchuk/nunchuk.h"

NunchukInput* nunchuk = NunchukInput::getInstance();

int main(void)
{
	sei();
	while (1)
	{
		NI.nunchuk_get();
		setLed();
		_delay_ms(10);
	}
}

