#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../../Adafruit_GFX.h"	// Core graphics library
#include "../../Adafruit_ILI9341.h"	// Hardware-specific library

#include "../../bomber.h"

Character* Character::instance = 0;

Character::Character() {}

Character* Character::getInstance() {
	if (instance == 0) {
		instance = new Character();
	}
	return instance;
}

//Maakt de character zichtbaar op het scherm.
void Character::init(int _y, int _x, int _height, int _width, uint16_t _color) {
	x = _x;
	y = _y;
	height = _height;
	width = _width;
	color = _color;
	tft.fillRect(_y, _x, _height, _width, _color);
}

//Wordt gebruikt om verscheidene objecten te tekenen op het scherm.
//Bomb wordt expliciet getekend zodat die nooit overgetekend kan worden.
void Character::draw(int y, int x, int height, int width, uint16_t color) {
	tft.fillRect(bomb->bombY, bomb->bombX, height, width, ILI9341_RED);
	tft.fillRect(y, x, height, width, color);
}

//Er wordt gekeken welke kant de character op zou moeten gaan.
//Vervolgens wordt er bekeken of hij die kant wel op mag gaan.
//Eerst of de speler aan de rand van het scherm zit, daarna 'collision'
//Die bekijkt wat er om de speler heen staat en of de speler tegen een muur of bomb aan loopt.
//Daarna wordt de huidige positie zwart getekend.
//En als laatste wordt de nieuwe locatie van de speler getekend.
void Character::move(Direction dir) {
	if (dir == Character::UP) {
		if ((y + height) < 240 && collision(Character::UP)) {
			draw(y, x, height, width, ILI9341_BLACK);
			y += height;
			draw(y, x, height, width, color);
		}
	}
	else if (dir == Character::RIGHT) {
		if ((x + width) < 320 && collision(Character::RIGHT)) {
			draw(y, x, height, width, ILI9341_BLACK);
			x += width;
			draw(y, x, height, width, color);
		}
	}
	else if (dir == Character::DOWN) {
		if ((y - height) > -1 && collision(Character::DOWN)) {
			draw(y, x, height, width, ILI9341_BLACK);
			y -= height;
			draw(y, x, height, width, color);
		}
	}
	else if (dir == Character::LEFT) {
		if ((x - width) > -1 && collision(Character::LEFT)) {
			draw(y, x, height, width, ILI9341_BLACK);
			x -= width;
			draw(y, x, height, width, color);
		}
	}
}

//Bekijkt of er in de gekozen richting een obstakel staat e.g. bom of muur.
bool Character::collision(Direction dir) {
	if (dir == Character::UP) {
		//Check Up
		if (((y + height) != bomb->bombY ||
			x != bomb->bombX) && bomb->exists) {
			return true;
		}
	}
	else if (dir == Character::RIGHT) {
		//Check Right
		if ((x + width) != bomb->bombX ||
			y != bomb->bombY) {
			return true;
		}
	}
	else if (dir == Character::DOWN) {
		//Check Down
		if ((y - height) != bomb->bombY ||
			x != bomb->bombX) {
			return true;
		}
	}
	else if (dir == Character::LEFT) {
		//Check Left
		if ((x - width) != bomb->bombX ||
			y != bomb->bombY) {
			return true;
		}
	}
	
	return false;
}