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
#include "libraries/IRlib/IR.h"

NunchukInput* nunchuk = NunchukInput::getInstance();

int main(void)
{
	sei();
	Serial.begin(115200);
	IR ir = IR(3, 38);
	ir.enableReceiver();
	while (1)
	{
		nunchuk->nunchuk_get();
		ir.send(0b01, 0x00, 8);
		ir.enableReceiver();
		if(ir.decode()) {
			Serial.print(ir.results.data, HEX);
		}
		_delay_ms(100);
	}
}
