/*
 * Main.cpp
 *
 * Created: 13/11/2019 11:35:58
 * Author : sbrol
 */

#include <avr/interrupt.h>
#include <util/delay.h>
#include <HardwareSerial.h>
#include "libraries/Nunchuk/nunchuk.h"

NunchukInput* nunchuk = NunchukInput::getInstance();

int main(void)
{
	sei();
	Serial.begin(9600);
	while (1)
	{
		nunchuk->nunchuk_get();
		_delay_ms(100);
	}
}
