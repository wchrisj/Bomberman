#ifndef CHARACTER.H
#define CHARACTER.H

#include "bomb.h"
class Character {
public:
	static Character* getInstance();

	enum Direction { UP, RIGHT, DOWN, LEFT };
	enum Direction direction;
	int prevX, x, prevY, y, height, width;
	uint16_t color;
	void init(int _y, int _x, int _height, int _width, uint16_t _color);
	void move(Direction dir);
	bool collision(Direction dir);
private:
	static Character* instance;
	Character();
};
#endif