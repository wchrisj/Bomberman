#ifndef BOMB.H
#define BOMB.H

class Bomb {
public:
	char bombX, bombY;
	void placeBomb();
	void explodeBomb();
	bool exists;
};
#endif