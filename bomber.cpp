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
#include "FinalScreen.h"

#define CHARACTER_MOVE 100
#define BOMB_EXPLODE 2000
#define BOMB_EXPLOSION 1500
#define COUNTER_RESEND_START 100
#define IR_CHECK 50
#define LOCAL_PLAYER 0
#define EXTERN_PLAYER 1

#define IR_RECEIVEPIN PIND2
#define IR_SENDFREQUENTIE 56

//F = Flag
//C = Counter
volatile char F_readNunchuk = 0;
volatile short F_bombExplosion[1];
volatile char F_readIR = 0;
volatile short C_readIR = 0;
volatile char C_charMove = 0;
volatile short C_bombs[1];
volatile uint8_t C_resendStart = 0;
volatile uint8_t cycle_staps = 0;
volatile uint8_t F_playerDeath = 0;
volatile status_t gameStatus;
volatile Homepage homepage; 			// maak homepage object aan
volatile Waitingpage waitingpage;		// maak waitingpage object aan

//Singleton design pattern
//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
NunchukInput* nunchuk = NunchukInput::getInstance();
Character localCharacter;
Character externCharacter;
Map mapGenerator;


LCD lcd = LCD();
FinalScreen finalscreen = FinalScreen();

//Game loop is 2 ms. Timer1 beheert dit.
//Elke 2ms wordt de input van de nunchuk gelezen zodat de input vlot werkt en geen delays oplevert
//Elke 200ms kan de character een stap zetten door de nunchuk uit te lezen
//Elke 4000ms wordt er een bom ge-explodeerd indien er een bom aanwezig is
ISR(TIMER1_COMPA_vect) { //Elke 2ms
	F_readNunchuk = 1;
	C_readIR++;
	if(C_readIR == IR_CHECK) {
		F_readIR = 1;
		C_readIR = 0;
	}
	switch(gameStatus) {
		case waiting:
			C_resendStart++;
			break;

		case playing:
			C_charMove++;
			break;
	}
}

int main (void)
{
	sei();
	Serial.begin(9600);

	gameStatus = notReady;	// status beginnen met notReady
	uint8_t identifierReceived = 0;
	IR ir = IR(IR_RECEIVEPIN, IR_SENDFREQUENTIE);
	ir.enableReceiver();

	homepage.HomepageText(&tft);

	gameTimerInit();

	uint16_t mapSeed = 0;

	while (1)
	{
		//Wire van nunchuk gebruikt een ISR. ISR in ISR mag niet.
		//Vandaar een handmatige flag zodat het wel kan.
		if (F_readNunchuk == 1) {
			nunchuk->nunchuk_get();
			F_readNunchuk = 0;
		} else {
			continue;
		}

		if(F_readIR == 1) {
			if(ir.decode()){			// IR uitlezen
				switch(ir.results.identifier){
					case IDENTIFIER_START:
						identifierReceived = 1;	// identifierReceived aanzetten
						break;
					case IDENTIFIER_PLAYER_LOC:
						if(gameStatus == playing){
							if(mapGenerator.map[ir.results.data] == TYPE_WALL){
								break;
							}
							for(uint16_t i = 0; i < MAP_SIZE; i++){
								if(mapGenerator.map[i] == TYPE_EXTERNPLAYER){
									mapGenerator.map[i] = TYPE_AIR; // De oude locatie van de andere speler is nu leeg(lucht)
									lcd.drawAir(i % MAP_WIDTH, (i-(i % MAP_WIDTH))/MAP_WIDTH);
								}
							}
							mapGenerator.map[ir.results.data] = TYPE_EXTERNPLAYER;
							
							externCharacter.x = (ir.results.data % MAP_WIDTH)*BLOCK_SIZE;
							externCharacter.y = ((ir.results.data-(externCharacter.x/BLOCK_SIZE))/ MAP_WIDTH)*BLOCK_SIZE;
							Serial.println(externCharacter.x);
						}
						if(gameStatus == waiting){
							mapSeed = ir.results.data;
						}
						break;
					case IDENTIFIER_BOM_LOC:
						externCharacter.bomb.placeBomb((ir.results.data % MAP_WIDTH)*BLOCK_SIZE, (ir.results.data-((ir.results.data % MAP_WIDTH))/ MAP_WIDTH*BLOCK_SIZE));
						// externCharacter.bomb.placeBomb(48, 48);
						break;
					// case IDENTIFIER_PLAYER_DEAD:
					// 	break;

				}
				ir.resumeReceiver();	// gaat volgende signaal lezen
			}
			F_readIR = 0;
		}

		switch(gameStatus){
			case notReady:								// als status is notReady, dan
				if (nunchuk->status.C == 1){			// kijken of de Z is ingedrukt met de Nunchuk
					gameStatus = waiting; 				// zet status op waiting
					waitingpage.show(&tft);				// maak het wachtscherm
				}else {
					continue;
				}
				break;
			
			case waiting:											// Als de status waiting is, dan
				if(identifierReceived){								// De ontvangen data decoderen zodat er iets mee gedaan kan worden
					if(ir.results.identifier == IDENTIFIER_START){ 	// Als het IR signaal een start identifier stuurt
						gameStatus = playing;						// dan, Zet status om naar playing
						lcd.fillScreen(BG_COLOR);
						mapGenerator.createMap(0xFFFF);
						lcd.drawMap();
						localCharacter.init(16, 16, ILI9341_YELLOW, TYPE_LOCALPLAYER);
						externCharacter.init(16, 16, ILI9341_YELLOW, TYPE_EXTERNPLAYER);
						lcd.statusBar();
					}
				} else if(C_resendStart == COUNTER_RESEND_START){ // Opnieuw een startsignaal versturen?
					C_resendStart = 0; 
					cycle_staps++;							// cycle_staps verhogen met +1
					if (cycle_staps == MAXDOTS){			// Als cycle_staps de maximale aantal punten heeft geprint 
						cycle_staps = 0;					// dan, cycle_staps resetten naar 0
					}
					waitingpage.cycle(&tft, &cycle_staps);	// functie printen van dots
					ir.send(IDENTIFIER_START,0,0);			// Verzend IR signaal met alleen een identifier

					ir.enableReceiver();					// Zet de IR-ontvanger aan
					continue;
				}else{
					continue;
				}
				break;
			
			case playing:
				// Player bewegingen
				if(C_charMove == CHARACTER_MOVE) { //200ms (100ticks * 2ms = 200ms)
					C_charMove = 0; // Reset timer
					int16_t newPos = -1; // -1 als er niet bewogen is, anders bevat hij de waarde van de nieuwe locatie
					if (nunchuk->status.UP == 1) {
						newPos = localCharacter.move(Character::UP);
					}
					else if (nunchuk->status.RIGHT == 1) {
						newPos = localCharacter.move(Character::RIGHT);
					}
					else if (nunchuk->status.DOWN == 1) {
						newPos = localCharacter.move(Character::DOWN);
					}
					else if (nunchuk->status.LEFT == 1) {
						newPos = localCharacter.move(Character::LEFT);
					}
					if(newPos >= 0){ // Is er bewogen?
						ir.send(IDENTIFIER_PLAYER_LOC, newPos, BITLENGTH_PLAYER_LOC);
						ir.enableReceiver();
					}

					// Plaats een bom
					if (nunchuk->status.Z == 1) {
						if(!localCharacter.bomb.exists) {
							uint16_t bombPos = localCharacter.bomb.placeBomb(localCharacter.x, localCharacter.y);
							ir.send(IDENTIFIER_BOM_LOC, bombPos, BITLENGTH_BOM_LOC);
							ir.enableReceiver();
						}
					}
					if(localCharacter.health == 0){
						finalscreen.LoseScreen(&tft);
						gameStatus = notReady;
						 if(localCharacter.health > 0){
							finalscreen.WinScreen(&tft);
						}
					}

					draw();
				}

				// Bommen
				if(localCharacter.bomb.exists) {
					C_bombs[LOCAL_PLAYER]++;
					if(C_bombs[LOCAL_PLAYER] == BOMB_EXPLOSION) {
						F_bombExplosion[LOCAL_PLAYER] = 1;
					}
				}
				if(externCharacter.bomb.exists) {
					C_bombs[EXTERN_PLAYER]++;
					if(C_bombs[EXTERN_PLAYER] == BOMB_EXPLOSION) {
						F_bombExplosion[EXTERN_PLAYER] = 1;
					}
				}
				
				if (C_bombs[LOCAL_PLAYER] == BOMB_EXPLODE) { //4seconden
					C_bombs[LOCAL_PLAYER] = 0;
					F_bombExplosion[LOCAL_PLAYER] = 0;
					localCharacter.bomb.explodeBomb(&F_playerDeath);
				}
				if (C_bombs[EXTERN_PLAYER] == BOMB_EXPLODE) {
					C_bombs[EXTERN_PLAYER] = 0;
					F_bombExplosion[EXTERN_PLAYER] = 0;
					externCharacter.bomb.explodeBomb(&F_playerDeath);
				}

				if(F_playerDeath){
					F_playerDeath = 0;
					ir.send(IDENTIFIER_PLAYER_DEAD, 0, BITLENGTH_PLAYER_DEAD);
					ir.enableReceiver();
				}
			break;
		}
	}

	return(0);
}

//Initalizeer timer1 voor de gameclock naar Compare A register elke 2ms
void gameTimerInit() {
	TCNT1  = 0;						// Zet de eigen counter op 0
	OCR1A = 31999;					// Zet de top op 31999
	TCCR1B |= (1 << WGM12);			// Zet op CTC mode
	TCCR1B |= (1 << CS10);			// Zet prescaler = 1 (geen prescaler)
	TIMSK1 |= (1 << OCIE1A);		// Zet de interrupt aan
}

void draw() {
	localCharacter.bomb.calculateBombRange();
	externCharacter.bomb.calculateBombRange();
	//Tekent localCharacter op het scherm.
	if((localCharacter.prevX != localCharacter.x) || (localCharacter.prevY != localCharacter.y)) {	
		lcd.drawAir(localCharacter.prevX / BLOCK_SIZE, localCharacter.prevY / BLOCK_SIZE);
	}
  
	lcd.drawPlayer(localCharacter.x / BLOCK_SIZE, localCharacter.y / BLOCK_SIZE, PLAYER_1); // mss later eerst nieuwe tekenen en dan pas oude weghalen
	lcd.drawPlayer(externCharacter.x / BLOCK_SIZE, externCharacter.y / BLOCK_SIZE, PLAYER_2); // mss later eerst nieuwe tekenen en dan pas oude weghalen
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
				case TYPE_EXTERNPLAYER:
					lcd.drawPlayer(x / BLOCK_SIZE, y / BLOCK_SIZE, PLAYER_2);
					break;
			}
		}
	}

	if(externCharacter.bomb.exists == true) {
		if(F_bombExplosion[EXTERN_PLAYER] == 1) {
			for(char i = 0; i < BOMB_TILES; i++ ) {
				//Teken elk blokje rood tenzij het -1 is.
				if(externCharacter.bomb.bomb_area[i] != -1) {
					short x = (externCharacter.bomb.bomb_area[i] % MAP_WIDTH) * BLOCK_SIZE;
					short y = ((externCharacter.bomb.bomb_area[i] - (externCharacter.bomb.bomb_area[i] % MAP_WIDTH)) / MAP_WIDTH) * BLOCK_SIZE;
					if(externCharacter.bomb.bomb_area[i+1] == -1 && i % 2 == 1) {
						lcd.drawExplosion(x / BLOCK_SIZE, y / BLOCK_SIZE, i+1);
					} else {
						lcd.drawExplosion(x / BLOCK_SIZE, y / BLOCK_SIZE, i);
					}
				}
			}
		} else {
			//Bom is geplaatst maar nog niet ge-explodeerd
			lcd.drawBomb(externCharacter.bomb.x / BLOCK_SIZE, externCharacter.bomb.y / BLOCK_SIZE);
		}
	} else {
		//Bom bestaat niet, dus teken elk blokje de juiste kleur volgens de map
		for(char i = 0; i < BOMB_TILES; i++ ) {
			short x = (externCharacter.bomb.bomb_area[i] % MAP_WIDTH) * BLOCK_SIZE;
			short y = ((externCharacter.bomb.bomb_area[i] - (externCharacter.bomb.bomb_area[i] % MAP_WIDTH)) / MAP_WIDTH) * BLOCK_SIZE;

			char type = mapGenerator.map[externCharacter.bomb.bomb_area[i]]; //Lees uit wat voor type het object is volgens de map
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
				case TYPE_EXTERNPLAYER:
					lcd.drawPlayer(x / BLOCK_SIZE, y / BLOCK_SIZE, PLAYER_2);
					break;
			}
		}
	}
}