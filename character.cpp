#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Adafruit_GFX.h"	// Core graphics library
#include "Adafruit_ILI9341.h"	// Hardware-specific library

#include "character.h"

void Character::init(int _y, int _x, int _height, int _width, uint16_t _color) {
	x = _x;
	y = _y;
	height = _height;
	width = _width;
	color = _color;
	tft.fillRect(_y, _x, _height, _width, _color);
}

void Character::draw(int y, int x, int height, int width, uint16_t color) {
	tft.fillRect(bombY, bombX, height, width, ILI9341_RED);
	tft.fillRect(y, x, height, width, color);
}

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

void Character::placeBomb() {
	draw(bombY, bombX, height, width, ILI9341_BLACK);
	bombX = x;
	bombY = y;
	draw(y, x, height, width, ILI9341_RED);
}

bool Character::collision(Direction dir) {
	if (dir == Character::UP) {
		//Check Up
		if ((y + height) != bombY ||
			x != bombX) {
			return true;
		}
	}
	else if (dir == Character::RIGHT) {
		//Check Right
		if ((x + width) != bombX ||
			y != bombY) {
			return true;
		}
	}
	else if (dir == Character::DOWN) {
		//Check Down
		if ((y - height) != bombY ||
			x != bombX) {
			return true;
		}
	}
	else if (dir == Character::LEFT) {
		//Check Left
		if ((x - width) != bombX ||
			y != bombY) {
			return true;
		}
	}
	
	return false;
}