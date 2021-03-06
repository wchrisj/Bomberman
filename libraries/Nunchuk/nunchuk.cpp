/*
 * Nunchuk.cpp
 *
 * Created: 13/11/2019 11:35:58
 * Author : Simon Brolsma S1131109
 */
#include <Wire.h>
#include <HardwareSerial.h>
#include "nunchuk.h"
#define NUNCHUK_ADDR 0x52
#define LESS_THAN 64
#define NEUTRAL 128
#define MORE_THAN 192
#define NUNCHUK_BYTES 6

NunchukInput* NunchukInput::instance = 0;

NunchukInput::NunchukInput() {}

NunchukInput* NunchukInput::getInstance() {
	if(instance == 0) {
		instance = new NunchukInput();
	}
	return instance;
}

//0 = Horizontal
//1 = Vertical
//2 = Gyro
//3 = Gyro
//4 = Gyro
//5 = {3: Released}, {2: Z}, {1: C}, {0: Z+C}
uint8_t nunchuk_data[6];
NunchukInput::input status;

//Waardes van de Nunchuk opvragen
void NunchukInput::nunchuk_get() {
	Wire.beginTransmission(NUNCHUK_ADDR);
	//6 bytes opvragen van de nunchuk
	Wire.requestFrom(NUNCHUK_ADDR, NUNCHUK_BYTES);
	//Data opslaan
	if (Wire.available()) {
		for (uint8_t i = 0; i < NUNCHUK_BYTES && Wire.available(); i++) {
			nunchuk_data[i] = Wire.read();
		}
		//0x00 maakt de buffer schoon voor de volgende bytes
		Wire.write(0x00);
		Wire.endTransmission(true);
		processNunchukData();
	}
}

//Opgevraagde data verwerken voor een betere leesbaarheid
void NunchukInput::processNunchukData() {
	//Neutrale X positie
	if (nunchuk_data[0] == NEUTRAL) {
		status.RIGHT = 0;
		status.LEFT = 0;
	}
	//Thumbstick naar rechts
	else if (nunchuk_data[0] >= MORE_THAN) {
		status.RIGHT = 1;
		status.LEFT = 0;
	}
	//Thumbstick naar links
	else if (nunchuk_data[0] <= LESS_THAN) {
		status.RIGHT = 0;
		status.LEFT = 1;
	}

	//Neutrale Y positie
	if (nunchuk_data[1] == NEUTRAL) {
		status.UP = 0;
		status.DOWN = 0;
	}
	//Thumbstick naar boven
	else if (nunchuk_data[1] >= MORE_THAN) {
		status.UP = 1;
		status.DOWN = 0;
	}
	//Thumbstick naar onderen
	else if (nunchuk_data[1] <= LESS_THAN) {
		status.UP = 0;
		status.DOWN = 1;
	}

	//Knop Z&C zijn niet ingedrukt
	if (nunchuk_data[5] == 3) {
		status.Z = 0;
		status.C = 0;
	}
	//Knop Z is ingedrukt
	else if (nunchuk_data[5] == 2) {
		status.Z = 1;
		status.C = 0;
	}
	//Knop C is ingedrukt
	else if (nunchuk_data[5] == 1) {
		status.Z = 0;
		status.C = 1;
	}
	//Knop Z&C zijn beiden ingedrukt
	else if (nunchuk_data[5] == 0) {
		status.Z = 1;
		status.C = 1;
	}
}
