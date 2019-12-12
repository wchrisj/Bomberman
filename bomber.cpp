#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"
#include "libraries/IRlib/IR.h"
#include "bomber.h"
#include "Homepage.h"
#include "Waitingpage.h"

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
volatile bool gestart = false;
volatile uint8_t cycle_staps = 0;

//Singleton design pattern
//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
NunchukInput* nunchuk = NunchukInput::getInstance();
Character localCharacter;
Character externCharacter;
Map mapGenerator;


LCD lcd = LCD();

//Game loop is 2 ms. Timer1 beheert dit.
//Elke 2ms wordt de input van de nunchuk gelezen zodat de input vlot werkt en geen delays oplevert
//Elke 200ms kan de character een stap zetten door de nunchuk uit te lezen
//Elke 4000ms wordt er een bom ge-explodeerd indien er een bom aanwezig is
ISR(TIMER1_COMPA_vect) { //Elke 2ms
	F_readNunchuk = 1;
	if(status == playing) {
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
}

int main (void)
{
	sei();
	Serial.begin(9600);

	IR ir = IR(2, 56);
	ir.enableReceiver();

	Homepage homepage = Homepage(&tft); 			// maak homepage object aan
	Waitingpage waitingpage = Waitingpage(&tft); 	// maak waitingpage object aan
	homepage.HomepageText(&tft);

	gameTimerInit();

	status_t status;
	status = notReady;	// status beginnen met notReady
	uint8_t identifierReceived = 0;

	while (1)
	{
		//Gametimer moet naar 15ms
		if( ir.decode()){			// IR uitlezen
			identifierReceived = 1;	// identifierReceived aanzetten
			ir.resumeReceiver();	// gaat volgende signaal lezen
		}
		//Wire van nunchuk gebruikt een ISR. ISR in ISR mag niet.
		//Vandaar een handmatige flag zodat het wel kan.
		if (F_readNunchuk == 1) {
			F_readNunchuk = 0;
			nunchuk->nunchuk_get();
		} else {
			continue;
		}

		if (status == notReady){					// als status is notReady, dan
			if (nunchuk->status.Z == 1){			// kijken of de Z is ingedrukt met de Nunchuk
				if(!gestart) {						// als niet gestart, dan
					gestart = true;					// zet gestart op true
					continue;
				}
				status = waiting; 					// zet status op waiting
				waitingpage.show(&tft);				// maak het wachtscherm
			}else {
				continue;
			}
		}
		if(status == waiting){									// Als de status waiting is, dan
			Serial.println("Waiting");
			if(identifierReceived){									// De ontvangen data decoderen zodat er iets mee gedaan kan worden
				if(ir.results.identifier == IDENTIFIER_START){ 	// Als het IR signaal een start identifier stuurt
				Serial.println("IDENTIFIER");
					status = playing;							// dan, Zet status om naar playing
					Serial.println("MAP MAKEN");
					mapGenerator.createMap(0xFFFF);
					lcd.drawMap();
					localCharacter.init(16, 16, ILI9341_YELLOW);
					lcd.statusBar();
				}
			} else{ 
				_delay_ms(100); //DIT MOET EEN GAME TICK WORDEN #################################################
				cycle_staps++;							// cycle_staps verhogen met +1
				if (cycle_staps == MAXDOTS){			// Als cycle_staps de maximale aantal punten heeft geprint 
					cycle_staps = 0;					// dan, cycle_staps resetten naar 0
				}
				waitingpage.cycle(&tft, &cycle_staps);	// functie printen van dots
				ir.send(IDENTIFIER_START,0,0);			// Verzend IR signaal met alleen een identifier

				ir.enableReceiver();					// Zet de IR-ontvanger aan
				continue;
			}
		}
		// if(status == playing){
		// 	//## hier moet het "SPEL / GAMELOOP"
		// }
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
		lcd.drawAir(localCharacter.prevX / BLOCK_SIZE, localCharacter.prevY / BLOCK_SIZE);
	}
  
	lcd.drawPlayer(localCharacter.x / BLOCK_SIZE, localCharacter.y / BLOCK_SIZE, PLAYER_1); // mss later eerst nieuwe tekenen en dan pas oude weghalen
	//tft.fillRect(localCharacter.x, localCharacter.y, localCharacter.height, localCharacter.width, ILI9341_YELLOW);
	//Tekent de bom van localCharacter op het scherm

	if(localCharacter.bomb.exists == true) {
		if(F_bombExplosion[LOCAL_PLAYER] == 1) {
			for(char i = 0; i < BOMB_TILES; i++ ) {
				//Teken elk blokje rood tenzij het -1 is.
				if(localCharacter.bomb.bomb_area[i] != -1) {
					short x = (localCharacter.bomb.bomb_area[i] % MAP_WIDTH) * BLOCK_SIZE;
					short y = ((localCharacter.bomb.bomb_area[i] - (localCharacter.bomb.bomb_area[i] % MAP_WIDTH)) / MAP_WIDTH) * BLOCK_SIZE;
					if(localCharacter.bomb.bomb_area[i+1] == -1 && i % 2 == 1) {
						lcd.drawExplosion(x / BLOCK_SIZE, y / BLOCK_SIZE, i+1);
					} else {
						lcd.drawExplosion(x / BLOCK_SIZE, y / BLOCK_SIZE, i);
					}
				}
			}
		} else {
			//Bom is geplaatst maar nog niet ge-explodeerd
			lcd.drawBomb(localCharacter.bomb.x / BLOCK_SIZE, localCharacter.bomb.y / BLOCK_SIZE);
		}
	} else {

		//Bom bestaat niet, dus teken elk blokje de juiste kleur volgens de map
		for(char i = 0; i < BOMB_TILES; i++ ) {
			short x = (localCharacter.bomb.bomb_area[i] % MAP_WIDTH) * BLOCK_SIZE;
			short y = ((localCharacter.bomb.bomb_area[i] - (localCharacter.bomb.bomb_area[i] % MAP_WIDTH)) / MAP_WIDTH) * BLOCK_SIZE;

			char type = mapGenerator.map[localCharacter.bomb.bomb_area[i]]; //Lees uit wat voor type het object is volgens de map
			switch(type) {
				case TYPE_AIR:
					lcd.drawAir(x / BLOCK_SIZE, y / BLOCK_SIZE);
					break;
				case TYPE_WALL:
					lcd.drawWall(x / BLOCK_SIZE, y / BLOCK_SIZE);
					break;
				case TYPE_CRATE:
					lcd.drawCrate(x / BLOCK_SIZE, y / BLOCK_SIZE);
					break;
				case TYPE_LOCALPLAYER:
					lcd.drawPlayer(x / BLOCK_SIZE, y / BLOCK_SIZE, PLAYER_1);
					break;
			}
		}
	}
}