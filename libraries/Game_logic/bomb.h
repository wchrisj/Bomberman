#ifndef BOMB.H
#define BOMB.H

class Bomb {
public:
	int bombX, bombY;
	void placeBomb(int x, int y);
	void explodeBomb();
	bool exists;
private:
};

#endif