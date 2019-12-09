#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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
	prevX = _x;
	y = _y;
	prevY = _y;
	height = _height;
	width = _width;
	color = _color;
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
			prevY = y;
			prevX = x;
			y += height;
		}
	}
	else if (dir == Character::RIGHT) {
		if ((x + width) < 320 && collision(Character::RIGHT)) {
			prevY = y;
			prevX = x;
			x += width;
		}
	}
	else if (dir == Character::DOWN) {
		if ((y - height) > -1 && collision(Character::DOWN)) {
			prevY = y;
			prevX = x;
			y -= height;
		}
	}
	else if (dir == Character::LEFT) {
		if ((x - width) > -1 && collision(Character::LEFT)) {
			prevY = y;
			prevX = x;
			x -= width;
		}
	}
}

//Bekijkt of er in de gekozen richting een obstakel staat e.g. bom of muur.
bool Character::collision(Direction dir) {

	if (dir == Character::UP) {
		//Check Up
		return true;
		// if(bombs[0].exists) {
		// 	if((y + height) == bombs[0].bombY &&
		// 		x != bombs[0].bombX) {
		// 		return true;
		// 	} else if ((y == bombs[0].bombY) &&
		// 			   (x == bombs[0].bombX))  {
		// 		return true;
		// 	}
		// } else {
		// 	return true;
		// }
	}
	else if (dir == Character::RIGHT) {
		//Check Right
		return true;
		// if(bombs[0].exists) {
		// 	if((x + width) == bombs[0].bombX &&
		// 		y != bombs[0].bombY) {
		// 		return true;
		// 	}
		// } else {
		// 	return true;
		// }
	}
	else if (dir == Character::DOWN) {
		//Check Down
		return true;
		// if(bombs[0].exists) {
		// 	if((y - height) == bombs[0].bombY &&
		// 		x != bombs[0].bombX) {
		// 		return true;
		// 	} else if (y == bombs[0].bombY &&
		// 			   x == bombs[0].bombX)  {
		// 		return true;
		// 	}
		// } else {
		// 	return true;
		// }
	}
	else if (dir == Character::LEFT) {
		//Check Left
		return true;
		// if(bombs[0].exists) {
		// 	if((x - width) == bombs[0].bombX &&
		// 		y != bombs[0].bombY) {
		// 		return true;
		// 	}
		// } else {
		// 	return true;
		// }
	}
	
	return false;
}