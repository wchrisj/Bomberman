/*
 * Nunchuk_data.h
 *
 * Created: 13/11/2019 11:35:58
 * Author : Simon Brolsma S1131109
 */
#ifndef NUNCHUK_H
#define NUNCHUK_H
class NunchukInput {
public:
	static NunchukInput* getInstance();

	struct input {
		int UP, RIGHT, DOWN, LEFT, Z, C;
	};
	input status;

	void nunchuk_get();
private:
	static NunchukInput* instance;
	NunchukInput();
	void processNunchukData();
};
#endif