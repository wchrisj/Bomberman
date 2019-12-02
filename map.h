#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#define FREEZONE_PLACES_COUNT 16 // 2 freezones, 8 plekken per freezone
#define PART_COUNT 7 // Er zijn 7 gebieden
#define MAP_SIZE 285 // 15*19

#define LAYOUT_CRATES_0 0x4147221
#define LAYOUT_CRATES_1 0x8A27B07
#define LAYOUT_CRATES_2 0x2FA630A0
#define LAYOUT_CRATES_3 0x4A25382

#define LAYOUT_FREEZONE_0 16,17,18,31,33,46,47,48,236,237,238,251,253,266,267,268

#define TYPE_AIR 0
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