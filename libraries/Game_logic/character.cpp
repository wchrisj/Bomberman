#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../../bomber.h"

#define convertPosition (x/16)+(y/16)*15

#define TYPE_AIR 0              // Types objecten die in het speelveld kunnen liggen
#define TYPE_WALL 1
#define TYPE_CRATE 2
#define TYPE_BOMB 3
#define TYPE_LOCALPLAYER 4
#define TYPE_EXTERNPLAYER 5

int mapArr[285] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,};
Character* Character::instance = 0;

Character::Character() {}

Character* Character::getInstance() {
	if (instance == 0) {
		instance = new Character();
	}
	return instance;
}

//Maakt de character zichtbaar op het scherm.
void Character::init(int _height, int _width, uint16_t _color) {
	for(int i = 0; i < 285; i++) {
		if(mapArr[i] == 4) {
			Serial.println(i);
			x = i;
			y = i;
			prevX = x;
			prevY = y;
		}
	}
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
		if ((y - height) > -1 && collision(Character::UP)) {
			prevY = y;
			prevX = x;
			mapArr[convertPosition-15] = TYPE_LOCALPLAYER;
			y -= height;
			mapArr[convertPosition+15] = TYPE_AIR;
		}
	}
	else if (dir == Character::RIGHT) {
		if ((x + width) < 240 && collision(Character::RIGHT)) {
			prevY = y;
			prevX = x;
			mapArr[convertPosition+1] = TYPE_LOCALPLAYER;
			x += width;
			mapArr[convertPosition-1] = TYPE_AIR;
		}
	}
	else if (dir == Character::DOWN) {
		if ((y + height) < 320 && collision(Character::DOWN)) {
			prevY = y;
			prevX = x;
			mapArr[convertPosition+15] = TYPE_LOCALPLAYER;
			y += height;
			mapArr[convertPosition-15] = TYPE_AIR;
		}
	}
	else if (dir == Character::LEFT) {
		if ((x - width) > -1 && collision(Character::LEFT)) {
			prevY = y;
			prevX = x;
			mapArr[convertPosition-1] = TYPE_LOCALPLAYER;
			x -= width;
			mapArr[convertPosition+1] = TYPE_AIR;
		}
	}
}

//Bekijkt of er in de gekozen richting een obstakel staat e.g. bom of muur.
bool Character::collision(Direction dir) {
	if (dir == Character::UP) {
		//Check Up
		int entity = mapArr[convertPosition-15];
		if(entity == TYPE_AIR) {
			Serial.println("air");
			return true;
		} else {
			Serial.println("indestructible");
			return false;
		}
	}
	else if (dir == Character::RIGHT) {
		//Check Right
		int entity = mapArr[convertPosition+1];
		if(entity == TYPE_AIR) {
			Serial.println("air");
			return true;
		} else {
			Serial.println("indestructible");
			return false;
		}
	}
	else if (dir == Character::DOWN) {
		//Check Down
		int entity = mapArr[convertPosition+15];
		if(entity == TYPE_AIR) {
			Serial.println("air");
			return true;
		} else {
			Serial.println("indestructible");
			return false;
		}
	}
	else if (dir == Character::LEFT) {
		int entity = mapArr[convertPosition-1];
		if(entity == TYPE_AIR) {
			Serial.println("air");
			return true;
		} else {
			Serial.println("indestructible");
			return false;
		}
	}
	return false;
}