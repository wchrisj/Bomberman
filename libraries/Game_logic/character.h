#ifndef CHARACTER.H
#define CHARACTER.H
#include "bomb.h"
class Character {
public:
	enum Direction { UP, RIGHT, DOWN, LEFT };
	enum Direction direction;
	int prevX, x, prevY, y, height, width;
	char health;
	uint16_t color;
	Bomb bomb;
	void init(int _height, int _width, uint16_t _color);
	void move(Direction dir);
	bool collision(Direction dir);
	Character();
private:
};
#endif