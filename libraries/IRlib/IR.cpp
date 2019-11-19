#include "IR.hpp"

uint8_t khz;

/*
    Constructor
    stelt de zendpin in en stelt de frequent van de timer op deze pin in
*/
IRsend::IRsend(uint8_t kHz){
    IRsettings.sendPin = (1 << PIND3); //Zet de zendpin als pin D3, deze hoort bij timer 2
    IRsend::setupTimer(kHz);
    IRsend::khz = kHz;
}

void IRsend::setupTimer(uint8_t kHz){
    //Zet de interrupts tijdelijk uit
    cli();

    //Stel de timer in 
    //Fast PWM, top = OCR2A
    TCCR2A |= (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << WGM22)

    if(kHz == 38){
        //Zet prescaler = 8
        TCCR2B |= (1<<CS21);

        //Zet OCR2A = 52
        OCR2A = 52;
    }else if(kHz == 56){
        //Zet prescaler = 8
        TCCR2B |= (1<<CS21);

        //Zet OCR2A = 36
        OCR2A = 36;
    }

    //Zet de interrupts weer aan
    sei();
}

void IRsend::send(uint16_t data, uint8_t len){
    //Zet de timer goed
    IRsend::setupTimer(IRsend::khz);

    //Zend start bit
    IRsend::sendStartBit();

    //Zend data en bereken paritybit
    uint8_t highBits = 0;
    for(uint8_t i = 0; i < len; i++){
        if(data & (1 << i)){
            IRsend::sendHigh();
            highBits++;
        }else{
            IRsend::sendLow();
        }
    }

    //Zend partitybit
    if(highBits & 1){ //Er is een oneven aantal eentjes
        IRsend::sendHigh();
    }else{
        IRsend::sendLow();
    }
}

void IRsend::sendStartBit(){

    _delay_us(5);
}

void IRsend::sendHigh(){

}

void IRsend::sendLow(){

}