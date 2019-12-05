#include "map.h";

uint8_t map[MAP_SIZE];

/*
    Deze functie kan worden aangeroepen van buiten af en zorgt ervoor dat 'map' gevuld wordt met de waarde die uit de seed berekend kan worden
    Hij plaatst eerste crates, daarna plaatst hij een 2 freezones, dan plaatst hij muren, hierna plaatst hij de spelers
*/
void Map::createMap(uint16_t seed){ 
    layout_t layout = createLayout(seed);
  
    for(int place = 0; place<MAP_SIZE; place++){
        for(uint8_t partCounter = 0; partCounter<PART_COUNT; partCounter++){
            if(placeCrates(&layout.parts[partCounter], place)){
                map[place] = TYPE_CRATE;
            }
        }
        for(int i = 0; i<FREEZONE_PLACES_COUNT; i++){
            map[layout.freezone[i]] = TYPE_AIR; // Freezone
        }
        createWalls(MAP_WIDTH, MAP_HEIGHT);
        map[layout.spawnPlayer1] = TYPE_LOCALPLAYER;
        map[layout.spawnPlayer2] = TYPE_EXTERNPLAYER;
    } 
}

/*
    Deze functie stuurt een 32 bits getal waarvan elke bit aangeeft als er een crate staat op een bepaalde positie
*/
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

/*
    Deze functie maakt een gebied, op basis van een linkerbovenhoek en een rechteronderhoek
    crates is het getal wat in de getCrateLayout functie opgehaald word
    counter houd bij op welke plek in het crates getal hij zit
*/
layoutPart_t Map::createLayoutPart(uint16_t leftTop, uint16_t rightBottom, uint32_t crates){
    layoutPart_t temp;
    temp.leftTop = leftTop;
    temp.rightBottom = rightBottom;
    temp.crates = crates;
    temp.counter = 0;
    return temp;
}

/*
    Deze functie maakt de layout aan
    Op basis van de 2 MSB wordt een bepaalde layout gekozen, hierin staan de verdeling van de gebieden, de freezones en de spawnpunten
    De rest van de 14 bits worden gebruik voor de gebieden, de 2 LSB voor gebieden A enz.
*/
layout_t Map::createLayout(uint16_t seed){
    uint16_t mask = (3 << SEED_MASK_LAYOUT);    // Welke layout is gekozen?
    layout_t temp;                              // Hierin wordt de layout opgebouwd
    uint16_t a[PART_CORNER_COUNT];              // Deze wordt gebruikt voor de hoeken van de gebieden

    if(((seed & mask) >> SEED_MASK_LAYOUT) == 0){       // Is het layout 0?
        uint16_t tempje[] = {LAYOUT_FREEZONE_0};        // Kan dit korter?
        for(int i = 0; i<FREEZONE_PLACES_COUNT; i++){   // Hier wordt de freezone opgeslagen
            temp.freezone[i] = tempje[i];
        }
        uint16_t tempje2[] = {LAYOUT_0};                // Kan dit korter?
        for(int i = 0; i<PART_CORNER_COUNT; i++){       // Hier worden de hoeken van de gebieden opgeslagen
            a[i] = tempje2[i];
        }
        temp.spawnPlayer1 = LAYOUT_0_SPAWN_1;           // Zet de spawn punten
        temp.spawnPlayer2 = LAYOUT_0_SPAWN_2;
    }else if(((seed & mask)) >> SEED_MASK_LAYOUT == 1){ // Is het layout 1?
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
    }else if(((seed & mask)) >> SEED_MASK_LAYOUT == 2){ // Is het layout 2?
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
    }else if(((seed & mask)) >> SEED_MASK_LAYOUT == 3){ // Is het layout 3?
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

    // Hier worden de gebieden berekend
    layoutPart_t layoutA = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_A], a[LAYOUT_LOWERRIGHT_CORNER_PART_A], getCrateLayout(seed, SEED_MASK_PART_A));
    layoutPart_t layoutB = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_B], a[LAYOUT_LOWERRIGHT_CORNER_PART_B], getCrateLayout(seed, SEED_MASK_PART_B));
    layoutPart_t layoutC = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_C], a[LAYOUT_LOWERRIGHT_CORNER_PART_C], getCrateLayout(seed, SEED_MASK_PART_C));
    layoutPart_t layoutD = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_D], a[LAYOUT_LOWERRIGHT_CORNER_PART_D], getCrateLayout(seed, SEED_MASK_PART_D));
    layoutPart_t layoutE = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_E], a[LAYOUT_LOWERRIGHT_CORNER_PART_E], getCrateLayout(seed, SEED_MASK_PART_E));
    layoutPart_t layoutF = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_F], a[LAYOUT_LOWERRIGHT_CORNER_PART_F], getCrateLayout(seed, SEED_MASK_PART_F));
    layoutPart_t layoutG = createLayoutPart(a[LAYOUT_UPPERLEFT_CORNER_PART_G], a[LAYOUT_LOWERRIGHT_CORNER_PART_G], getCrateLayout(seed, SEED_MASK_PART_G));
    
    layoutPart_t layoutParts[] = {layoutA, layoutB, layoutC, layoutD, layoutE, layoutF, layoutG};
    for(int i = 0; i<PART_COUNT; i++){  // Hier worden de gebieden opgeslagen
        temp.parts[i] = layoutParts[i];
    }
    return temp;
}

/*
    Deze functie zet muren om het veld heen en zet muren om het blok in het veld
*/
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

/*
    Deze functie kijkt als er op een specifiek plaats(place) en crate ligt
    In layout->crates zit een 32 bits getal waarin een 1 een crate betekend
    Hij kijkt eerst als de plek wel in het gebied ligt, hierna kijkt hij als er een crate ligt op place, dan returned hij een 1, anders een 0
*/
uint8_t Map::placeCrates(layoutPart_t *layout, uint16_t place){
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