#ifndef IR_h
#define IR_h

#include "stdint.h"

#define TIME_LOW 600
#define TIME_0 600
#define TIME_1 1200
#define TIME_START 2400

typedef
    struct{
        uint8_t receivePin;
        uint8_t sendPin;
    }
IRsettings_t;

class IR{
    public:
        IR(uint8_t receivePin, uint8_t kHz);
        void send(uint16_t data, uint8_t len);
        void enableReceiver();

        IRsettings_t IRsettings;

    private:
        void setupSendTimer(uint8_t kHz);
        void sendStartBit();
        void sendHigh();
        void sendLow();

        uint8_t khz;
};

#endif