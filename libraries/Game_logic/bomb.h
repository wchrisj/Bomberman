#ifndef BOMB.H
#define BOMB.H

#define BOMB_TILES 9

class Bomb {
public:
	short x, y;								//		    2
	char bomb_range;						//		    1
	short bomb_area[BOMB_TILES];			//		8 7 0 3 4
	void placeBomb(short _x, short _y);		//		    5
	void explodeBomb();						//		    6
	void calculateBombRange();
	bool exists;
private:
};

#endif