/*
 * Nunchuk.cpp
 *
 * Created: 13/11/2019 11:35:58
 * Author : Simon Brolsma S1131109
 */
#include <Wire.h>
#include "nunchuk.h"
#define NUNCHUK_ADDR 0x52

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
	Wire.requestFrom(NUNCHUK_ADDR, 6);
	//Data opslaan
	if (Wire.available()) {
		for (uint8_t i = 0; i < 6 && Wire.available(); i++) {
			nunchuk_data[i] = Wire.read();
		}
		Wire.write(0x00);
		Wire.endTransmission(true);
		processNunchukData();
	}
}

//Opgevraagde data verwerken voor een betere leesbaarheid
void NunchukInput::processNunchukData() {
	//Neutrale X positie
	if (nunchuk_data[0] == 128) {
		status.RIGHT = 0;
		status.LEFT = 0;
	}
	//Thumbstick naar rechts
	else if (nunchuk_data[0] >= 212) {
		status.RIGHT = 1;
		status.LEFT = 0;
		Serial.println("RIGHT");
	}
	//Thumbstick naar links
	else if (nunchuk_data[0] <= 44) {
		status.RIGHT = 0;
		status.LEFT = 1;
		Serial.println("LEFT");
	}

	//Neutrale Y positie
	if (nunchuk_data[1] == 128) {
		status.UP = 0;
		status.DOWN = 0;
	}
	//Thumbstick naar boven
	else if (nunchuk_data[1] >= 212) {
		status.UP = 1;
		status.DOWN = 0;
		Serial.println("UP");
	}
	//Thumbstick naar onderen
	else if (nunchuk_data[1] <= 44) {
		status.UP = 0;
		status.DOWN = 1;
		Serial.println("DOWN");
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
		Serial.println("Z");
	}
	//Knop C is ingedrukt
	else if (nunchuk_data[5] == 1) {
		status.Z = 0;
		status.C = 1;
		Serial.println("C");
	}
	//Knop Z&C zijn beiden ingedrukt
	else if (nunchuk_data[5] == 0) {
		status.Z = 1;
		status.C = 1;
		Serial.println("Z & C");
	}
}
