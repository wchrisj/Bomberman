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
Adafruit_ImageReader reader;     // Class w/image-reading functions
Adafruit_Image       img;        // An image loaded into RAM
int32_t              width  = 0, // BMP image dimensions
                     height = 0;
NunchukInput* nunchuk = NunchukInput::getInstance();
Character* character = Character::getInstance();
Bomb bombs[1];

ISR(TIMER1_COMPA_vect) { //Elke 2ms
	F_readNunchuk = 1;
	C_charMove++;
	if(bombs[0].exists) {
		C_bombs[0]++;
	}
	if(bombs[1].exists) {
		C_bombs[1]++;
	}
	if(C_charMove == CHARACTER_MOVE) { //200ms
		C_charMove = 0;
		Serial.println("boop");
		if (nunchuk->status.UP == 1) {
			character->move(Character::UP);
			Serial.println("UP");
		}
		else if (nunchuk->status.RIGHT == 1) {
			character->move(Character::RIGHT);
			Serial.println("RIGHT");
		}
		else if (nunchuk->status.DOWN == 1) {
			character->move(Character::DOWN);
			Serial.println("DOWN");
		}
		else if (nunchuk->status.LEFT == 1) {
			character->move(Character::LEFT);
			Serial.println("LEFT");
		}
		if (nunchuk->status.Z == 1) {
			if(!bombs[0].exists) {
				bombs[0].placeBomb();
			}
		}
		draw();
	}

	if (C_bombs[0] == BOMB_EXPLODE) { //12seconden
		C_bombs[0] = 0;
		bombs[0].explodeBomb();
	}
	if (C_bombs[1] == BOMB_EXPLODE) {
		C_bombs[1] = 0;
		bombs[1].explodeBomb();
	}
}

int main (void)
{
	sei();
	_delay_ms(50);
	Serial.begin(9600);
	LCD lcd = LCD();

	Serial.print(F("Initializing SD card..."));
	SD.begin(SD_CS); //als niet lukt loopt die vast, fail detection werkt toch niet
	Serial.println(F("OK!"));

	lcd.drawMap();
	lcd.drawStatusBar();
	// dit uiteindelijk allemaal in LCD
	reader.drawBMP("/h1.bmp", tft, BLOCK_SIZE * 2, LENGTH - BLOCK_SIZE); //tekent ❤️
	reader.drawBMP("/h1.bmp", tft, BLOCK_SIZE * 3, LENGTH - BLOCK_SIZE);
	reader.drawBMP("/h1.bmp", tft, BLOCK_SIZE * 4, LENGTH - BLOCK_SIZE);
	reader.drawBMP("/h1.bmp", tft, BLOCK_SIZE * 8, LENGTH - BLOCK_SIZE);
	reader.drawBMP("/h1.bmp", tft, BLOCK_SIZE * 9, LENGTH - BLOCK_SIZE);
	reader.drawBMP("/h1.bmp", tft, BLOCK_SIZE * 10, LENGTH - BLOCK_SIZE);

	_delay_ms(1000);
	lcd.updateLives(5);

	character->init(0, 0, 16, 16, ILI9341_YELLOW);
	gameTimerInit();

	while (1)
	{
		// Wire van nunchuk gebruikt een ISR. ISR in ISR mag niet.
		// Vandaar een handmatige flag zodat het wel kan.
		if (F_readNunchuk == 1) {
			F_readNunchuk = 0;
			nunchuk->nunchuk_get();
		}
	}

	return(0);
}

void gameTimerInit() { //dit mss in gamelogic?
	TCCR1A = 0; // set entire TCCR1A register to 0
	TCCR1B = 0; // same for TCCR1B
	TCNT1 = 0; // initialize counter value to 0
	// set compare match register for 500 Hz increments
	OCR1A = 31999; // = 16000000 / (1 * 500) - 1 (must be <65536) 500hz 2ms
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS12, CS11 and CS10 bits for 1 prescaler
	TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
}

void draw() {  //mss dat deze functie in LCD moet?
	if((character->prevX != character->x) || (character->prevY != character->y)) {
		reader.drawBMP("/p1.bmp", tft, character->y, character->x); //tekent player 1 vanaf SD kaart
		tft.fillRect(character->prevY, character->prevX, character->height, character->width, BG_COLOR);
	}
	if(bombs[0].exists == true) {
		reader.drawBMP("/b.bmp", tft, bombs[0].bombY, bombs[0].bombX); //tekent player 1 vanaf SD kaart
	} else {
		tft.fillRect(bombs[0].bombY, bombs[0].bombX, character->height, character->width, BG_COLOR);
	}
}