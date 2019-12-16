#ifndef BOMB_H
#define BOMB_H

#define BOMB_TILES 9

class Bomb {
public:
	short x, y;									//		    2
	char bomb_range;							//		    1
	short bomb_area[BOMB_TILES];				//		8 7 0 3 4
	uint16_t placeBomb(short _x, short _y);		//		    5
	void explodeBomb(uint8_t *flag);			//		    6
	void calculateBombRange();
	bool exists;
private:
};

#endif