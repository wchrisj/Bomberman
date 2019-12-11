#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"
#include "bomber.h"

#define CHARACTER_MOVE 100
#define BOMB_EXPLODE 2000
#define BOMB_EXPLOSION 1500
#define LOCAL_PLAYER 0
#define EXTERN_PLAYER 1

void gameTimerInit();
void draw();

//F = Flag
//C = Counter
volatile char F_readNunchuk = 0;
volatile short F_bombExplosion[1];
volatile char C_charMove = 0;
volatile short C_bombs[1];

//Singleton design pattern
//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
NunchukInput* nunchuk = NunchukInput::getInstance();
Character localCharacter;
Character externCharacter;
Map mapGenerator;

//Game loop is 2 ms. Timer1 beheert dit.
//Elke 2ms wordt de input van de nunchuk gelezen zodat de input vlot werkt en geen delays oplevert
//Elke 200ms kan de character een stap zetten door de nunchuk uit te lezen
//Elke 4000ms wordt er een bom ge-explodeerd indien er een bom aanwezig is
ISR(TIMER1_COMPA_vect) { //Elke 2ms
	F_readNunchuk = 1;
	C_charMove++;
	if(localCharacter.bomb.exists) {
		C_bombs[LOCAL_PLAYER]++;
		if(C_bombs[LOCAL_PLAYER] == BOMB_EXPLOSION) {
			F_bombExplosion[LOCAL_PLAYER] = 1;
		}
	}
	if(externCharacter.bomb.exists) {
		C_bombs[1]++;
		if(C_bombs[1] == BOMB_EXPLOSION) {
			F_bombExplosion[1] = 1;
		}
	}
	if(C_charMove == CHARACTER_MOVE) { //200ms (100ticks * 2ms = 200ms)
		C_charMove = 0;
		if (nunchuk->status.UP == 1) {
			localCharacter.move(Character::UP);
			Serial.println("UP");
		}
		else if (nunchuk->status.RIGHT == 1) {
			localCharacter.move(Character::RIGHT);
			Serial.println("RIGHT");
		}
		else if (nunchuk->status.DOWN == 1) {
			localCharacter.move(Character::DOWN);
			Serial.println("DOWN");
		}
		else if (nunchuk->status.LEFT == 1) {
			localCharacter.move(Character::LEFT);
			Serial.println("LEFT");
		}
		if (nunchuk->status.Z == 1) {
			if(!localCharacter.bomb.exists) {
				localCharacter.bomb.placeBomb(localCharacter.x, localCharacter.y);
			}
		}
		draw();
	}

	if (C_bombs[LOCAL_PLAYER] == BOMB_EXPLODE) { //4seconden
		C_bombs[LOCAL_PLAYER] = 0;
		F_bombExplosion[LOCAL_PLAYER] = 0;
		localCharacter.bomb.explodeBomb();
	}
	if (C_bombs[EXTERN_PLAYER] == BOMB_EXPLODE) {
		C_bombs[EXTERN_PLAYER] = 0;
		F_bombExplosion[EXTERN_PLAYER] = 0;
		externCharacter.bomb.explodeBomb();
	}
}

int main (void)
{
	sei();
	Serial.begin(9600);
	tft.begin();
	mapGenerator.createMap(0xABCD);
	LCD lcd = LCD();
	lcd.drawMap();
	localCharacter.init(16, 16, ILI9341_YELLOW);
	gameTimerInit();
	while (1)
	{
		//Wire van nunchuk gebruikt een ISR. ISR in ISR mag niet.
		//Vandaar een handmatige flag zodat het wel kan.
		if (F_readNunchuk == 1) {
			F_readNunchuk = 0;
			nunchuk->nunchuk_get();
		}
	}

	return(0);
}

//Initalizeer timer1 voor de gameclock naar Compare A register elke 2ms
void gameTimerInit() {
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	// compare match register A op 500hz zetten, aka 2ms.
	OCR1A = 31999;
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
	TIMSK1 |= (1 << OCIE1A);
}

void draw() {
	localCharacter.bomb.calculateBombRange();
	//Tekent localCharacter op het scherm.
	if((localCharacter.prevX != localCharacter.x) || (localCharacter.prevY != localCharacter.y)) {	
		tft.fillRect(localCharacter.prevX, localCharacter.prevY, localCharacter.height, localCharacter.width, ILI9341_BLACK);
	}
	tft.fillRect(localCharacter.x, localCharacter.y, localCharacter.height, localCharacter.width, ILI9341_YELLOW);

	//Tekent de bom van localCharacter op het scherm
	if(localCharacter.bomb.exists == true) {
		if(F_bombExplosion[LOCAL_PLAYER] == 1) {
			for(char i = 0; i < BOMB_TILES; i++ ) {
				//Teken elk blokje rood tenzij het -1 is.
				if(localCharacter.bomb.bomb_area[i] != -1) {
					short x = (localCharacter.bomb.bomb_area[i] % MAP_WIDTH) * BLOCK_SIZE;
					short y = ((localCharacter.bomb.bomb_area[i] - (localCharacter.bomb.bomb_area[i] % MAP_WIDTH)) / MAP_WIDTH) * BLOCK_SIZE;
					tft.fillRect(x, y, localCharacter.height, localCharacter.width, ILI9341_RED);
				}
			}
		} else {
			//Bom is geplaatst maar nog niet ge-explodeerd
			tft.fillRect(localCharacter.bomb.x, localCharacter.bomb.y, localCharacter.height, localCharacter.width, ILI9341_RED);
		}
	} else {
		//Bom bestaat niet, dus teken elk blokje de juiste kleur volgens de map
		for(char i = 0; i < BOMB_TILES; i++ ) {
			short x = (localCharacter.bomb.bomb_area[i] % MAP_WIDTH) * BLOCK_SIZE;
			short y = ((localCharacter.bomb.bomb_area[i] - (localCharacter.bomb.bomb_area[i] % MAP_WIDTH)) / MAP_WIDTH) * BLOCK_SIZE;

			char type = mapGenerator.map[localCharacter.bomb.bomb_area[i]]; //Lees uit wat voor type het object is volgens de map
			switch(type) {
				case TYPE_AIR:
					tft.fillRect(x, y, localCharacter.height, localCharacter.width, ILI9341_BLACK);
					break;
				case TYPE_WALL:
					tft.fillRect(x, y, localCharacter.height, localCharacter.width, ILI9341_LIGHTGREY);
					break;
				case TYPE_CRATE:
					tft.fillRect(x, y, localCharacter.height, localCharacter.width, ILI9341_ORANGE);
					break;
				case TYPE_LOCALPLAYER:
					tft.fillRect(x, y, localCharacter.height, localCharacter.width, ILI9341_YELLOW);
					break;
			}
		}
	}
}