#ifndef BOMB.H
#define BOMB.H

class Bomb {
public:
	short x, y;								//		    2
	char bomb_range;						//		    1
	short bomb_area[9];						//		8 7 0 3 4
	void placeBomb(short _x, short _y);		//		    5
	void explodeBomb();						//		    6
	void calculateBombRange();
	bool exists;
private:
};

#endif