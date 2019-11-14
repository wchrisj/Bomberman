/*
 * Nunchuk.cpp
 *
 * Created: 13/11/2019 11:35:58
 * Author : Simon Brolsma S1131109
 */
#include <Wire.h>
#include "nunchuk_data.h"
#define NUNCHUK_ADDR 0x52

//0 = Horizontal
//1 = Vertical
//2 = Gyro
//3 = Gyro
//4 = Gyro
//5 = {3: Released}, {2: Z}, {1: C}, {0: Z+C}
uint8_t nunchuk_data[6];
input status;

void processNunchukData();

//Waardes van de Nunchuk opvragen
void nunchuk_get() {
	Wire.beginTransmission(NUNCHUK_ADDR);
	//6 bytes opvragen van de nunchuk
	Wire.requestFrom(NUNCHUK_ADDR, 6);
	//Data opslaan
	for (uint8_t i = 0; i < 6 && Wire.available(); i++) {
		nunchuk_data[i] = Wire.read();
	}
	Wire.write(0x00);
	Wire.endTransmission(true);
	processNunchukData();
}

//Opgevraagde data verwerken voor een betere leesbaarheid
void processNunchukData() {
	//Neutrale X positie
	if (nunchuk_data[0] == 128) {
		status.RIGHT = 0;
		status.LEFT = 0;
	}
	//Thumbstick naar rechts
	else if (nunchuk_data[0] >= 200) {
		status.RIGHT = 1;
		status.LEFT = 0;
	}
	//Thumbstick naar links
	else if (nunchuk_data[0] <= 100) {
		status.RIGHT = 0;
		status.LEFT = 1;
	}

	//Neutrale Y positie
	if (nunchuk_data[1] == 128) {
		status.UP = 0;
		status.DOWN = 0;
	}
	//Thumbstick naar boven
	else if (nunchuk_data[1] >= 200) {
		status.UP = 1;
		status.DOWN = 0;
	}
	//Thumbstick naar onderen
	else if (nunchuk_data[1] <= 100) {
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