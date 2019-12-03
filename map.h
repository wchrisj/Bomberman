#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#define FREEZONE_PLACES_COUNT 18 // 2 freezones, 9 plekken per freezone
#define PART_COUNT 7 // Er zijn 7 gebieden
#define MAP_SIZE 285 // 15*19

#define LAYOUT_CRATES_0 0x4147221   // Hoe leg ik dit uit?
#define LAYOUT_CRATES_1 0x8A27B07
#define LAYOUT_CRATES_2 0x2FA630A0
#define LAYOUT_CRATES_3 0x4A25382

#define LAYOUT_FREEZONE_0 16,17,18,31,32,33,46,47,48,236,237,238,251,252,253,266,267,268   // De plekken waar geen crates mogen spawnen, moet FREEZONE_PLACES_COUNT aantal plekken bevatten
#define LAYOUT_FREEZONE_1 26,27,28,41,42,43,56,57,58,226,227,228,241,242,243,256,257,258 
#define LAYOUT_FREEZONE_2 121,122,123,136,137,138,151,152,153,131,132,133,146,147,148,161,162,163 
#define LAYOUT_FREEZONE_3 21,22,23,36,37,38,51,52,53,231,232,233,246,247,248,261,262,263 

#define LAYOUT_0 19, 58, 64, 103, 109, 148, 154, 193, 226, 265, 61, 183, 196, 223   // Dit zijn de linkerbovenhoek en de rechteronderhoek van alle 7 gebieden in de verschillende layouts
#define LAYOUT_1 16, 55, 61, 100, 106, 145, 151, 190, 229, 268, 71, 193, 196, 223
#define LAYOUT_2 16, 109, 20, 114, 25, 118, 124, 160, 166, 259, 170, 164, 175, 168 
#define LAYOUT_3 16, 95, 106, 170, 181, 260, 66, 218, 24, 103, 114, 178, 189, 168

#define TYPE_AIR 0              // Types objecten die in het speelveld kunnen liggen
#define TYPE_WALL 1
#define TYPE_CRATE 2
#define TYPE_BOMB 3
#define TYPE_LOCALPLAYER 4
#define TYPE_EXTERNPLAYER 5

typedef
    struct{
        uint16_t leftTop;                           // Het coordinaat linksboven
        uint16_t rightBottom;                       // Het coordinaat rechtsonder
        uint32_t crates;                            // De lijst me de locatie van de crates in per gebied
        uint8_t counter;                            // Deze counter telt hoeveel plekken van het gebied al berekend zijn
    }
layoutPart_t;

typedef
  struct{
      uint16_t freezone[FREEZONE_PLACES_COUNT];     // Wat zijn de freezone punten
      layoutPart_t parts[PART_COUNT];               // Waar liggen de verschillende gebieden
  }
layout_t;

class Map{
    public:
        void createMap(uint16_t seed);
        uint8_t map[MAP_SIZE];

    private:
        uint32_t getCrateLayout(uint16_t seed, uint8_t layout);
        layoutPart_t createLayoutPart(uint16_t leftTop, uint16_t rightBottom, uint32_t crates);
        layout_t createLayout(uint16_t seed);
        void createWalls(int width, int height);
        uint8_t fillPart(layoutPart_t *layout, uint16_t place);
        uint8_t cellIs(uint16_t input[], uint8_t size, uint16_t place);
};

#endif