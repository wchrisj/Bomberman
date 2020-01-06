#ifndef IR_h
#define IR_h

#include "stdint.h"

#define TIME_LOW 600    // De tijd dat het signaal HIGH is
#define TIME_0 600      // De tijd dat het signaal LOW is voor een 0
#define TIME_1 1200     // De tijd dat het signaal LOW is voor een 1
#define TIME_START 2400 // De tijd dat het signaal LOW is voor een startbit

#define HIGH 1
#define LOW  0
#define IDENTIFIER_MASK 0x03
#define IDENTIFIER_LENGTH 2

#define IDENTIFIER_MAP 0
#define IDENTIFIER_START 1
#define IDENTIFIER_PLAYER_LOC 0
#define IDENTIFIER_BOM_LOC 2
#define IDENTIFIER_PLAYER_DEAD 3

#define BITLENGTH_MAP 16
#define BITLENGTH_START 0
#define BITLENGTH_PLAYER_LOC 9
#define BITLENGTH_BOM_LOC 9
#define BITLENGTH_PLAYER_DEAD 0

#define USECPERTICK 50 // Hoeveel tijd zit er tussen de metingen?
#define GAP_TICKS 100 // Hoeveel ticks mag het signaal achter elkaar LOW zijn, als er geen data verstuurt word is het signaal LOW
#define MAX_ZERO_VALUE 900 // Wat is de hoogste waarde die een 0 mag hebben -> hoger is het een 1

#define TIMER_COUNT_TOP  (F_CPU * USECPERTICK / 1000000) // Bij een prescaler van 0, wat is de top van de timer zodat er tussen 2 pulsen USECPERTICK tussen zit

#define RAWBUF  101  // Maximale lengte van de buffer

typedef
    enum STATE{
        IDLE,
        RECEIVING,  // Is de receiver bezig met het meten van een signaal
        STOP,       // Is de data ontvangen en klaar om gedecode te worden
        OVERFLOW    // Is er meer data ontvangen dan er kan worden opgeslagen
    }
state_t;

typedef
    struct {
        uint8_t  identifier;   // Hier staat voor wie de data is
        uint16_t data;         // Hier staat de ontvangen data
        uint8_t  dataBits;     // Hier staat de hoeveelheid dataBits -> geen startbit, geen paritybit
    }
decoderesults_t;

typedef
    struct {
        state_t       receiveState;    // Hier staat in welke status de ontvanger verkeerd
        uint8_t       rawlen;          // De lengte van de buffer
        unsigned int  timer;           // De timer 1 is gelijk aan USECPERTICK.
        unsigned int  rawbuf[RAWBUF];  // De gemeette tijden
        uint8_t       overflow;        // Is er een overflow?
    }
irparams_t;

typedef
    struct{
        uint8_t receivePin;    // De pin waarmee infrarood ontvangen word
        uint8_t frequency;     // De frequentie waarmee het infrarood signaal verstuurd word
        uint8_t sendPin;       // De pin waarmee data verstuurt wordt
    }
IRsettings_t;

class IR{
    public:
        IR(uint8_t receivePin, uint8_t kHz);
        void send(uint8_t identifier, uint16_t data, uint8_t len);
        void enableReceiver();
        void resumeReceiver();
        uint8_t decode();

        decoderesults_t results;

    private:
        void setupSendTimer(uint8_t kHz);
        void sendStartBit();
        void sendHigh();
        void sendLow();
};

#endif
