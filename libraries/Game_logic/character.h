#ifndef CHARACTER_H
#define CHARACTER_H
#include "bomb.h"
class Character {
public:
	enum Direction { UP, RIGHT, DOWN, LEFT };
	enum Direction direction;
	int prevX, x, prevY, y, height, width;
	char health;
	uint16_t color;
	Bomb bomb;
	void init(int _height, int _width, uint16_t _color, uint8_t _player);
	int16_t move(Direction dir);
	bool collision(Direction dir);
private:
};
#endif