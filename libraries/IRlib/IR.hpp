#ifndef IR_h
#define IR_h

#include "stdint.h"

typedef
    struct{
        uint8_t receivePin;
        uint8_t sendPin;
    }
IRsettings_t;

//Zorg dat iedereen bij de IR settings kan
extern IRsettings_t IRsettings;

class IRreceive{
    public:
        IRreceive(uint8_t receivePin);
        void enable();

    private:
};

class IRsend{
    public:
        IRsend(uint8_t kHz);
        void send(uint16_t data, uint8_t len);
        static const uint8_t test = 0;

    private:
        void setupTimer(uint8_t kHz);
        void sendStartBit();
        void sendHigh();
        void sendLow();

        uint8_t khz;
};

#endif