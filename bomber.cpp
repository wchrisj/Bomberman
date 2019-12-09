#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "libraries/AdafruitGFX/Adafruit_GFX.h"
#include "libraries/ILI9341/Adafruit_ILI9341.h"
#include "bomber.h"

#define CHARACTER_MOVE 100
#define BOMB_EXPLODE 2000

void gameTimerInit();
void draw();

//F = Flag
//C = Counter
volatile char F_readNunchuk = 0;
volatile char C_charMove = 0;
volatile int C_bombs[1];

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
	C_charMove++;
	if(localCharacter.bomb.exists) {
		C_bombs[0]++;
	}
	if(externCharacter.bomb.exists) {
		C_bombs[1]++;
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

	if (C_bombs[0] == BOMB_EXPLODE) { //4seconden
		C_bombs[0] = 0;
		localCharacter.bomb.explodeBomb();
	}
	if (C_bombs[1] == BOMB_EXPLODE) {
		C_bombs[1] = 0;
		externCharacter.bomb.explodeBomb();
	}
}

int main (void)
{
	sei();
	Serial.begin(9600);
	//tft.begin();
	mapGenerator.createMap(0xFFFF); //Seed = 1
	//LCD lcd = LCD();
	lcd.drawMap();
	localCharacter.init(16, 16, ILI9341_YELLOW);

	lcd.drawPlayer(11, 15, 0x0526);
	lcd.drawPlayer(12, 15, 0xF800);

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
	if((localCharacter.prevX != localCharacter.x) || (localCharacter.prevY != localCharacter.y)) {	
		lcd.drawAir(localCharacter.prevX, localCharacter.prevY);
	}
	lcd.drawPlayer(localCharacter.x / 16, localCharacter.y / 16, 0x8810); // mss later eerst nieuwe tekenen en dan pas oude weghalen
	//tft.fillRect(localCharacter.x, localCharacter.y, localCharacter.height, localCharacter.width, ILI9341_YELLOW);
	if(localCharacter.bomb.exists == true) {
		tft.fillRect(localCharacter.bomb.bombX, localCharacter.bomb.bombY, localCharacter.height, localCharacter.width, ILI9341_RED);
	} else {
		lcd.drawAir(localCharacter.bomb.bombX, localCharacter.bomb.bombY);
	}
}