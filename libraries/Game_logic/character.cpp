#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../../bomber.h"

#define convertPosition (x/BLOCK_SIZE)+(y/BLOCK_SIZE)*MAP_WIDTH	//Ruwe positie bijv 160x160 omzetten naar een positie in de map array

//Maakt de character zichtbaar op het scherm.
void Character::init(int _height, int _width, uint16_t _color, uint8_t _player) {
	for(short i = 0; i < MAP_SIZE; i++) {
		if(mapGenerator.map[i] == _player) {
			x = (i % MAP_WIDTH)*BLOCK_SIZE;
			y = ((i-(i%MAP_WIDTH))/MAP_WIDTH)*BLOCK_SIZE;
			prevX = x;
			prevY = y;
		}
	}
	health = 3;
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
int16_t Character::move(Direction dir) {
	prevY = y;
	prevX = x;
	if (dir == Character::UP) {
		if (collision(Character::UP)) {
			uint16_t newPos = convertPosition-MAP_WIDTH;
			mapGenerator.map[newPos] = TYPE_LOCALPLAYER;
			y -= height;
			mapGenerator.map[convertPosition+MAP_WIDTH] = TYPE_AIR;
			return newPos;
		}
	}
	else if (dir == Character::RIGHT) {
		if (collision(Character::RIGHT)) {
			uint16_t newPos = convertPosition+1;
			mapGenerator.map[newPos] = TYPE_LOCALPLAYER;
			x += width;
			mapGenerator.map[convertPosition-1] = TYPE_AIR;
			return newPos;
		}
	}
	else if (dir == Character::DOWN) {
		if (collision(Character::DOWN)) {
			uint16_t newPos = convertPosition+MAP_WIDTH;
			mapGenerator.map[newPos] = TYPE_LOCALPLAYER;
			y += height;
			mapGenerator.map[convertPosition-MAP_WIDTH] = TYPE_AIR;
			return newPos;
		}
	}
	else if (dir == Character::LEFT) {
		if (collision(Character::LEFT)) {
			uint16_t newPos = convertPosition-1;
			mapGenerator.map[newPos] = TYPE_LOCALPLAYER;
			x -= width;
			mapGenerator.map[convertPosition+1] = TYPE_AIR;
			return newPos;
		}
	}
	return -1; // Hier mag hij nooit komen
}

//Bekijkt of er in de gekozen richting een obstakel staat e.g. bom of muur.
bool Character::collision(Direction dir) {
	if (dir == Character::UP) {
		//Check Up
		int entity = mapGenerator.map[convertPosition-MAP_WIDTH];
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
		int entity = mapGenerator.map[convertPosition+1];
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
		int entity = mapGenerator.map[convertPosition+MAP_WIDTH];
		if(entity == TYPE_AIR) {
			Serial.println("air");
			return true;
		} else {
			Serial.println("indestructible");
			return false;
		}
	}
	else if (dir == Character::LEFT) {
		int entity = mapGenerator.map[convertPosition-1];
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