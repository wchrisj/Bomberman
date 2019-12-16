#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../../bomber.h"

#define convertPosition (x/BLOCK_SIZE)+(y/BLOCK_SIZE)*MAP_WIDTH	//Ruwe positie bijv 160x160 omzetten naar een positie in de map array

//Update positie van de bom & zet de Flag voor de draw functie zodat die weet dat een bom wel/niet getekend moet worden
uint16_t Bomb::placeBomb(short _x, short _y) {
	x = _x;
	y = _y;
	exists = true;
	Serial.println("BOMB_PLACED");
	return convertPosition;
}

void Bomb::explodeBomb(uint8_t *flag) {
	for(char i = 0; i < BOMB_TILES; i++) {
		uint8_t type = mapGenerator.map[bomb_area[i]];	
		if(type == TYPE_CRATE) {
			mapGenerator.map[bomb_area[i]] = 0;
		} else if(type == TYPE_LOCALPLAYER) {
			localCharacter.health--;
			*flag = 1;
		} else if(type == TYPE_EXTERNPLAYER) {
			externCharacter.health--;
		}
	}

	exists = false;
	Serial.println("BOMB_REMOVED");
}

void Bomb::calculateBombRange() {
	for(char i = 0; i < BOMB_TILES; i++) {
		switch(i) {
			case 0: //Middenpunt
				bomb_area[i] = convertPosition;
				break;
			case 1: //Één plaats boven het midden
				if(convertPosition-MAP_WIDTH >= 0) {
					bomb_area[i] = convertPosition-MAP_WIDTH;
				}
				break;
			case 2: //Twee plaatsen boven het midden
				if(convertPosition-MAP_WIDTH >= 0) {
					bomb_area[i] = convertPosition-(MAP_WIDTH * 2);
				}
				break;
			case 3:	//Één plaats rechts van het midden
				if(convertPosition-MAP_WIDTH <= 285) {
					bomb_area[i] = convertPosition+1;
				}
				break;
			case 4: //Twee plaatsen naar het rechts van midden
				if(convertPosition-MAP_WIDTH <= 285) {
					bomb_area[i] = convertPosition+2;
				}
				break;
			case 5: //Één plaats beneden het midden
				if(convertPosition-MAP_WIDTH <= 285) {
					bomb_area[i] = convertPosition+MAP_WIDTH;
				}
				break;
			case 6: //Twee plaatsen onder het midden
				if(convertPosition-MAP_WIDTH <= 285) {
					bomb_area[i] = convertPosition+(MAP_WIDTH * 2);
				}
				break;
			case 7: //Één plaats links van het midden
				if(convertPosition-MAP_WIDTH >= 0) {
					bomb_area[i] = convertPosition-1;
				}
				break;
			case 8: //Twee plaatsen links van het midden
				if(convertPosition-MAP_WIDTH >= 0) {
					bomb_area[i] = convertPosition-2;
				}
				break;
		}
	}
	for(char i = 1; i < BOMB_TILES; i+=2) { //i+=2 zodat alleen de eerst aanliggende tiles worden bekeken
		uint8_t type = mapGenerator.map[bomb_area[i]];
		if(type == TYPE_WALL) {
			bomb_area[i] = -1;
			bomb_area[i+1] = -1;
		} else if(mapGenerator.map[bomb_area[i+1]] == TYPE_WALL) {
			bomb_area[i+1] = -1;
		} else if(type == TYPE_CRATE ||
				  type == TYPE_LOCALPLAYER ||
				  type == TYPE_EXTERNPLAYER) {
			bomb_area[i+1] = -1;
		}
	}
}