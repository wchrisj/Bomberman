#ifndef BOMB.H
#define BOMB.H

class Bomb {
public:
	static Bomb* getInstance();

	char bombX, bombY;
	void placeBomb();
	void explodeBomb();
	bool exists;
private:
	static Bomb* instance;
	Bomb();
};
#endif