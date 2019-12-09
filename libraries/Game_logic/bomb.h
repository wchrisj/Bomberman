#ifndef BOMB.H
#define BOMB.H

class Bomb {
public:
	Bomb();
	int bombX, bombY;
	void placeBomb();
	void explodeBomb();
	bool exists;
private:
};

#endif