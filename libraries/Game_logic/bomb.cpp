#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../../bomber.h"

Bomb::Bomb() {}

//Update positie van de bom & zet de Flag voor de draw functie zodat die weet dat een bom wel/niet getekend moet worden
void Bomb::placeBomb(int x, int y) {
	bombX = x;
	bombY = y;
	exists = true;
	Serial.println("BOMB_PLACED");
}

void Bomb::explodeBomb() {
	exists = false;
	Serial.println("BOMB_REMOVED");	
}