#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Adafruit_GFX.h"	// Core graphics library
#include "Adafruit_ILI9341.h"	// Hardware-specific library

#include "libraries/Game_logic/character.h"
#include "libraries/Nunchuk/nunchuk.h"

int main (void)
{
	Character character;
	NunchukInput Nunchuk;

	sei();
	_delay_ms(50);
	character.tft.begin();
	character.tft.fillScreen(ILI9341_BLACK);	

  character.init(0, 0, 20, 20, ILI9341_YELLOW);

  while (1)
  {
	  Nunchuk.nunchuk_get();
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
		  character.placeBomb();
	  }
	  _delay_ms(100);
  }

  return(0);
}

