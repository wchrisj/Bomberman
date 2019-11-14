/*
 * Nunchuk_data.h
 *
 * Created: 13/11/2019 11:35:58
 * Author : Simon Brolsma S1131109
 */
#ifndef NUNCHUK.H
#define NUNCHUK.H
struct input {
	int UP, RIGHT, DOWN, LEFT, Z, C;
};
extern input status;

extern void nunchuk_get();
#endif