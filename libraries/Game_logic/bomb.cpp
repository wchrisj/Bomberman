#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../../bomber.h"

#define convertPosition (x/16)+(y/16)*15	//Ruwe positie bijv 160x160 omzetten naar een positie in de map array

//Update positie van de bom & zet de Flag voor de draw functie zodat die weet dat een bom wel/niet getekend moet worden
void Bomb::placeBomb(short _x, short _y) {
	x = _x;
	y = _y;
	exists = true;
	Serial.println("BOMB_PLACED");
}

void Bomb::explodeBomb() {
	for(char i = 1; i < 9; i+=2) {
		uint8_t type = mapGenerator.map[bomb_area[i]];	
		if(type == TYPE_CRATE) {
			mapGenerator.map[bomb_area[i]] = 0;
		} else if(type == TYPE_LOCALPLAYER) {
			localCharacter.health--;
		} else if(type == TYPE_EXTERNPLAYER) {
			externCharacter.health--;
		}
	}

	exists = false;
	Serial.println("BOMB_REMOVED");
}

void Bomb::calculateBombRange() {
	for(char i = 0; i < 9; i++) {
		switch(i) {
			case 0: //Center position
				bomb_area[i] = convertPosition;
				break;
			case 1: //One above center
				if(convertPosition-MAP_WIDTH >= 0) {
					bomb_area[i] = convertPosition-MAP_WIDTH;
				}
				break;
			case 2: //Two above center
				if(convertPosition-MAP_WIDTH >= 0) {
					bomb_area[i] = convertPosition-(MAP_WIDTH * 2);
				}
				break;
			case 3:	//One to the right of center
				if(convertPosition-MAP_WIDTH <= 285) {
					bomb_area[i] = convertPosition+1;
				}
				break;
			case 4: //Two to the right of center
				if(convertPosition-MAP_WIDTH <= 285) {
					bomb_area[i] = convertPosition+2;
				}
				break;
			case 5: //One below center
				if(convertPosition-MAP_WIDTH <= 285) {
					bomb_area[i] = convertPosition+MAP_WIDTH;
				}
				break;
			case 6: //Two below center
				if(convertPosition-MAP_WIDTH <= 285) {
					bomb_area[i] = convertPosition+(MAP_WIDTH * 2);
				}
				break;
			case 7: //One to the left of center
				if(convertPosition-MAP_WIDTH >= 0) {
					bomb_area[i] = convertPosition-1;
				}
				break;
			case 8: //Two to the left of center
				if(convertPosition-MAP_WIDTH >= 0) {
					bomb_area[i] = convertPosition-2;
				}
				break;
		}
	}
	for(char i = 1; i < 9; i+=2) {
		uint8_t type = mapGenerator.map[bomb_area[i]];
		Serial.print(type);
		if(type == TYPE_WALL) {
			bomb_area[i] = -1;
			bomb_area[i+1] = -1;
		} else if(mapGenerator.map[bomb_area[i+1]] == TYPE_WALL) {
			bomb_area[i+1] = -1;
		} else if(type == TYPE_CRATE ||
				  type == TYPE_LOCALPLAYER ||
				  type == TYPE_EXTERNPLAYER) {
			bomb_area[i+1] = -1;
			if(type == TYPE_CRATE) {
				mapGenerator.map[bomb_area[i]] = 0;
			} else if(type == TYPE_LOCALPLAYER) {
				//Remove life
			} else if(type == TYPE_EXTERNPLAYER) {
				//Remove life
			}
		}
	}
	Serial.println();
}