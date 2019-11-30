#define TEST 16,17,18,31,33,46,47,48,236,237,238,251,253,266,267,268
uint16_t num[] = {TEST};

#define WALL 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,29,30,32,34,36,38,40,42,44,45,59,60,62,64,66,68,70,72,74,75,89,90,92,94,96,98,100,102,104,105,119
uint16_t wall[] = {WALL};

#define LAYOUT_CRATES_0 0x4147221
#define LAYOUT_CRATES_1 0x8A27B07
#define LAYOUT_CRATES_2 0x2FA630A0
#define LAYOUT_CRATES_3 0x4A25382

#define TYPE_AIR 0
#define TYPE_WALL 1
#define TYPE_CRATE 2
#define TYPE_BOMB 3
#define TYPE_LOCALPLAYER 4
#define TYPE_EXTERNPLAYER 5

typedef
    struct{
        uint16_t leftTop;
        uint16_t rightBottom;
        uint32_t crates;
        uint8_t a;
    }
layoutPart_t;

uint8_t layout[6][2];

uint8_t Map[285];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
    // put your main code here, to run repeatedly:
    createMap(5676);
    for(int y = 0; y<19; y++){
        for(int x = 0; x<15; x++){
            if(Map[x+y*15] < 2){
                Serial.print(" ");
            }else{
                Serial.print(Map[x+y*15]);
            }
            Serial.print(" ");
        }
        Serial.println("");
    }
    delay(100000);
}

uint32_t getCrateLayout(uint16_t seed, uint8_t layout){
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

void createMap(uint16_t seed){ 
    layoutPart_t layoutA;
    layoutA.leftTop = 19;
    layoutA.rightBottom = 58;
    layoutA.crates = getCrateLayout(seed, 0);
    layoutA.a = 0;

    layoutPart_t layoutB;
    layoutB.leftTop = 64;
    layoutB.rightBottom = 103;
    layoutB.crates = getCrateLayout(seed, 1);
    layoutB.a = 0;
    
    layoutPart_t layoutC;
    layoutC.leftTop = 109;
    layoutC.rightBottom = 148;
    layoutC.crates = getCrateLayout(seed, 2);
    layoutC.a = 0;
    
    layoutPart_t layoutD;
    layoutD.leftTop = 154;
    layoutD.rightBottom = 193;
    layoutD.crates = getCrateLayout(seed, 3);
    layoutD.a = 0;
    
    layoutPart_t layoutE;
    layoutE.leftTop = 226;
    layoutE.rightBottom = 265;
    layoutE.crates = getCrateLayout(seed, 4);
    layoutE.a = 0;
    
    layoutPart_t layoutF;
    layoutF.leftTop = 61;
    layoutF.rightBottom = 183;
    layoutF.crates = getCrateLayout(seed, 5);
    layoutF.a = 0;
    
    layoutPart_t layoutG;
    layoutG.leftTop = 196;
    layoutG.rightBottom = 223;
    layoutG.crates = getCrateLayout(seed, 6);
    layoutG.a = 0;

    layoutPart_t layoutParts[] = {layoutA, layoutB, layoutC, layoutD, layoutE, layoutF, layoutG};
  
    for(int place = 0; place<285; place++){
        for(uint8_t partCounter = 0; partCounter<7; partCounter++){
            if(int result = fillPart(&layoutParts[partCounter], place)){
                if(result == 1){
                    Map[place] = 0;
                }else{
                    Map[place] = TYPE_CRATE;
                }
            }
        }
        if(cellIs(num, sizeof(num), place)){
            Map[place] = TYPE_AIR; // Freezone
        }
        if(cellIs(wall, sizeof(wall), place)){
            Map[place] = TYPE_WALL; // Muren
        }
        Map[16] = TYPE_LOCALPLAYER;
        Map[268] = TYPE_EXTERNPLAYER;
    } 
}

uint8_t fillPart(layoutPart_t *layout, uint16_t place){
    for(uint16_t i = layout->leftTop; i<= layout->rightBottom; i++){ 
        if(place == i){
            if(layout->leftTop % 15 > i % 15 || layout->rightBottom % 15 < i % 15){
                return 0;
            }
            if(layout->crates & ((uint32_t)1 << layout->a)){
                layout->a++;
                return 2;
            }
            layout->a++;
            return 1;
        }
    }
    return 0;
}

uint8_t cellIs(uint16_t input[], uint8_t size, uint16_t place){
    uint8_t arraySize = size/2;       // 2 bytes per getal, dus arraysize = /2
    for(int i = 0; i<arraySize; i++){
        if(input[i] == place){
            return true;
        }
    }
    return false;
}