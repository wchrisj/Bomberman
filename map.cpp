#include "map.h";

uint8_t map[MAP_SIZE];

void Map::createMap(uint16_t seed){ 
    layout_t layout = createLayout(seed);
  
    for(int place = 0; place<MAP_SIZE; place++){
        for(uint8_t partCounter = 0; partCounter<PART_COUNT; partCounter++){
            if(fillPart(&layout.parts[partCounter], place)){
                map[place] = TYPE_CRATE;
            }
        }
        if(cellIs(layout.freezone, sizeof(layout.freezone), place)){
            map[place] = TYPE_AIR; // Freezone
        }
        createWalls(MAP_WIDTH, MAP_HEIGHT);
        map[layout.spawnPlayer1] = TYPE_LOCALPLAYER;
        map[layout.spawnPlayer2] = TYPE_EXTERNPLAYER;
    } 
}

uint32_t Map::getCrateLayout(uint16_t seed, uint8_t layout){
    uint16_t mask = (3 << layout);
    if(((seed & mask) >> layout) == 0){
        return LAYOUT_CRATES_0;
    }else if(((seed & mask)) >> layout == 1){
        return LAYOUT_CRATES_1;        
    }else if(((seed & mask)) >> layout == 2){
        return LAYOUT_CRATES_2;
    }else if(((seed & mask)) >> layout == 3){
        return LAYOUT_CRATES_3;
    }
}

layoutPart_t Map::createLayoutPart(uint16_t leftTop, uint16_t rightBottom, uint32_t crates){
    layoutPart_t temp;
    temp.leftTop = leftTop;
    temp.rightBottom = rightBottom;
    temp.crates = crates;
    temp.counter = 0;
    return temp;
}

layout_t Map::createLayout(uint16_t seed){
    uint16_t mask = (3 << SEED_MASK_LAYOUT);
    layout_t temp;
    uint16_t a[PART_CORNER_COUNT];

    if(((seed & mask) >> SEED_MASK_LAYOUT) == 0){
        uint16_t tempje[] = {LAYOUT_FREEZONE_0};        // Kan dit korter?
        for(int i = 0; i<FREEZONE_PLACES_COUNT; i++){
            temp.freezone[i] = tempje[i];
        }
        uint16_t tempje2[] = {LAYOUT_0};                // Kan dit korter?
        for(int i = 0; i<PART_CORNER_COUNT; i++){
            a[i] = tempje2[i];
        }
        temp.spawnPlayer1 = LAYOUT_0_SPAWN_1;           // Zet de spawn punten
        temp.spawnPlayer2 = LAYOUT_0_SPAWN_2;
    }else if(((seed & mask)) >> SEED_MASK_LAYOUT == 1){
        uint16_t tempje[] = {LAYOUT_FREEZONE_1};        // Kan dit korter?
        for(int i = 0; i<FREEZONE_PLACES_COUNT; i++){
            temp.freezone[i] = tempje[i];
        }
        uint16_t tempje2[] = {LAYOUT_0};                // Kan dit korter?
        for(int i = 0; i<PART_CORNER_COUNT; i++){
            a[i] = tempje2[i];
        }
        temp.spawnPlayer1 = LAYOUT_1_SPAWN_1;           // Zet de spawn punten
        temp.spawnPlayer2 = LAYOUT_1_SPAWN_2;
    }else if(((seed & mask)) >> SEED_MASK_LAYOUT == 2){
        uint16_t tempje[] = {LAYOUT_FREEZONE_2};        // Kan dit korter?
        for(int i = 0; i<FREEZONE_PLACES_COUNT; i++){
            temp.freezone[i] = tempje[i];
        }
        uint16_t tempje2[] = {LAYOUT_0};                // Kan dit korter?
        for(int i = 0; i<PART_CORNER_COUNT; i++){
            a[i] = tempje2[i];
        }
        temp.spawnPlayer1 = LAYOUT_2_SPAWN_1;           // Zet de spawn punten
        temp.spawnPlayer2 = LAYOUT_2_SPAWN_2;
    }else if(((seed & mask)) >> SEED_MASK_LAYOUT == 3){
        uint16_t tempje[] = {LAYOUT_FREEZONE_3};        // Kan dit korter?
        for(int i = 0; i<FREEZONE_PLACES_COUNT; i++){
            temp.freezone[i] = tempje[i];
        }
        uint16_t tempje2[] = {LAYOUT_0};                // Kan dit korter?
        for(int i = 0; i<PART_CORNER_COUNT; i++){
            a[i] = tempje2[i];
        }
        temp.spawnPlayer1 = LAYOUT_3_SPAWN_1;           // Zet de spawn punten
        temp.spawnPlayer2 = LAYOUT_3_SPAWN_2;
    }

    layoutPart_t layoutA = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_A], a[LAYOUT_LOWERRIGHT_CORNER_PART_A], getCrateLayout(seed, SEED_MASK_PART_A));
    layoutPart_t layoutB = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_B], a[LAYOUT_LOWERRIGHT_CORNER_PART_B], getCrateLayout(seed, SEED_MASK_PART_B));
    layoutPart_t layoutC = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_C], a[LAYOUT_LOWERRIGHT_CORNER_PART_C], getCrateLayout(seed, SEED_MASK_PART_C));
    layoutPart_t layoutD = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_D], a[LAYOUT_LOWERRIGHT_CORNER_PART_D], getCrateLayout(seed, SEED_MASK_PART_D));
    layoutPart_t layoutE = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_E], a[LAYOUT_LOWERRIGHT_CORNER_PART_E], getCrateLayout(seed, SEED_MASK_PART_E));
    layoutPart_t layoutF = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_F], a[LAYOUT_LOWERRIGHT_CORNER_PART_F], getCrateLayout(seed, SEED_MASK_PART_F));
    layoutPart_t layoutG = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_G], a[LAYOUT_LOWERRIGHT_CORNER_PART_G], getCrateLayout(seed, SEED_MASK_PART_G));
    
    layoutPart_t layoutParts[] = {layoutA, layoutB, layoutC, layoutD, layoutE, layoutF, layoutG};
    for(int i = 0; i<PART_COUNT; i++){
        temp.parts[i] = layoutParts[i];
    }
    return temp;
}

void Map::createWalls(int width, int height){
    for(int i = 0; i<width; i++){               // Teken horizontale muren(boven en onder)
        map[i] = TYPE_WALL;
        map[width*(height-1)+i] = TYPE_WALL;
    }
    for(int i = width; i<width*height; i+=width){ // Teken verticale muren(links en rechts)
        map[i] = TYPE_WALL;
        map[i+width-1] = TYPE_WALL;        
    }
    for(int i = 2; i<height; i+=2){               // Teken puntjes(om het blok)
        for(int j = 2; j<width; j+=2){
            map[i*width+j] = TYPE_WALL;
        }
    }
}

uint8_t Map::fillPart(layoutPart_t *layout, uint16_t place){
    for(uint16_t i = layout->leftTop; i<= layout->rightBottom; i++){ 
        if(place == i){
            if(layout->leftTop % 15 > i % 15 || layout->rightBottom % 15 < i % 15){
                return 0;   // Deze plek ligt niet in het goede gebied
            }
            if(layout->crates & ((uint32_t)1 << layout->counter)){
                layout->counter++;
                return 1;   // Het is een plek in het goede gebieden en hier ligt een crate
            }
            layout->counter++;
            return 0; // Het is een plek in het goede gebied, hier ligt alleen geen crate
        }
    }
    return 0;
}

uint8_t Map::cellIs(uint16_t input[], uint8_t size, uint16_t place){
    uint8_t arraySize = size/2;       // 2 bytes per getal, dus arraysize = /2
    for(int i = 0; i<arraySize; i++){
        if(input[i] == place){
            return true;
        }
    }
    return false;
}