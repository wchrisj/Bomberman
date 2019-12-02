#include "map.h";

uint8_t map[MAP_SIZE];

void Map::createMap(uint16_t seed){ 
    layout_t layout = createLayout(seed);
  
    for(int place = 0; place<285; place++){
        for(uint8_t partCounter = 0; partCounter<7; partCounter++){
            if(int result = fillPart(&layout.parts[partCounter], place)){
                if(result == 1){
                    map[place] = 0;
                }else{
                    map[place] = TYPE_CRATE;
                }
            }
        }
        if(cellIs(layout.freezone, sizeof(layout.freezone), place)){
            map[place] = TYPE_AIR; // Freezone
        }
        createWalls(15, 19);
        map[16] = TYPE_LOCALPLAYER;
        map[268] = TYPE_EXTERNPLAYER;
    } 
}

uint32_t Map::getCrateLayout(uint16_t seed, uint8_t layout){
    uint16_t mask = (3 << layout*2);
    if(((seed & mask) >> layout*2) == 0){
        return LAYOUT_CRATES_0;
    }else if(((seed & mask)) >> layout*2 == 1){
        return LAYOUT_CRATES_1;        
    }else if(((seed & mask)) >> layout*2 == 2){
        return LAYOUT_CRATES_2;
    }else if(((seed & mask)) >> layout*2 == 3){
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
    layoutPart_t layoutA = createLayoutPart(19, 58, getCrateLayout(seed, 0));
    layoutPart_t layoutB = createLayoutPart(64, 103, getCrateLayout(seed, 1));
    layoutPart_t layoutC = createLayoutPart(109, 148, getCrateLayout(seed, 2));
    layoutPart_t layoutD = createLayoutPart(154, 193, getCrateLayout(seed, 3));
    layoutPart_t layoutE = createLayoutPart(226, 265, getCrateLayout(seed, 4));
    layoutPart_t layoutF = createLayoutPart(61, 183, getCrateLayout(seed, 5));
    layoutPart_t layoutG = createLayoutPart(196, 223, getCrateLayout(seed, 6));
    
    layoutPart_t layoutParts[] = {layoutA, layoutB, layoutC, layoutD, layoutE, layoutF, layoutG};
    layout_t temp;
    for(int i = 0; i<7; i++){
        temp.parts[i] = layoutParts[i];
    }
    uint16_t tempje[] = {LAYOUT_FREEZONE_0};        // Kan dit korter?
    for(int i = 0; i<16; i++){
        temp.freezone[i] = tempje[i];
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
                return 0;
            }
            if(layout->crates & ((uint32_t)1 << layout->counter)){
                layout->counter++;
                return 2;
            }
            layout->counter++;
            return 1;
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