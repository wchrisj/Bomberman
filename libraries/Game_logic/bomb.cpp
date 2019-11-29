#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../../Adafruit_GFX.h"	// Core graphics library
#include "../../Adafruit_ILI9341.h"	// Hardware-specific library

#include "../../bomber.h"

Bomb::Bomb() {}

void Bomb::placeBomb() {
	bombX = character->x;
	bombY = character->y;
	exists = true;
	Serial.println("BOMB_PLACED");
}

void Bomb::explodeBomb() {
	exists = false;
	Serial.println("BOMB_REMOVED");		
}