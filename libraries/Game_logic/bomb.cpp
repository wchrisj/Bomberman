#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../../Adafruit_GFX.h"	// Core graphics library
#include "../../Adafruit_ILI9341.h"	// Hardware-specific library

#include "../../bomber.h"

Bomb* Bomb::instance = 0;

Bomb::Bomb() {}

Bomb* Bomb::getInstance() {
	if (instance == 0) {
		instance = new Bomb();
	}
	return instance;
}

void Bomb::placeBomb() {
	character->draw(bombY, bombX, character->height, character->width, ILI9341_BLACK);
	bombX = character->x;
	bombY = character->y;
	character->draw(character->y, character->x, character->height, character->width, ILI9341_RED);
	exists = true;
}

void Bomb::explodeBomb() {
	character->draw(bombY, bombX, character->height, character->width, ILI9341_BLACK);
	exists = false;
}