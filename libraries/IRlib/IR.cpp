#include "IR.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
    Constructor
    stelt de zendpin in en stelt de frequent van de timer op deze pin in
*/
IR::IR(uint8_t receivePin, uint8_t kHz){
    //Maak sender en receiver aan
    //Vul IRsettings

    IRsettings.sendPin = receivePin;

    IR::IRsettings.sendPin = (1 << PIND3); //Zet de zendpin als pin D3, deze hoort bij timer 2
    IR::setupSendTimer(kHz);
    IR::khz = kHz;
}

void IR::enableReceiver(){

}

void IR::setupSendTimer(uint8_t kHz){
    //Zet de interrupts tijdelijk uit
    cli();

    //Stel de timer in 
    //Fast PWM, top = OCR2A
    TCCR2A |= (1 << WGM20);
    TCCR2B |= (1 << WGM22);
    TCCR2B |= (1 << CS20); //Geen prescaling

    OCR2A = F_CPU / 2000 / kHz;
    OCR2B = (F_CPU / 2000 / kHz)/3;

    // if(kHz == 38){
    //     //Zet prescaler = 8
    //     TCCR2B |= (1<<CS21);

    //     //Zet OCR2A = 52
    //     OCR2A = 52;
    // }else if(kHz == 56){
    //     //Zet prescaler = 8
    //     TCCR2B |= (1<<CS21);

    //     //Zet OCR2A = 36
    //     OCR2A = 36;
    // }

    //Zet de interrupts weer aan
    sei();
}

void IR::send(uint16_t data, uint8_t len){
    //Zet de timer goed
    TIMSK2 = 0;
    DDRD |= (1 << DDD3);
    PIND &= ~(1 << DDD3);
    IR::setupSendTimer(IR::khz);

    //Zend start bit
    IR::sendStartBit();

    //Zend data en bereken paritybit
    uint8_t highBits = 0;
    for(uint8_t i = len-1; i >= 0; i--){
        if(data & (1 << i)){
            IR::sendHigh();
            highBits++;
        }else{
            IR::sendLow();
        }
        if(i == 0){
            break;
        }
    }

    //Zend partitybit
    if(highBits & 1){ //Er is een oneven aantal eentjes
        IR::sendHigh();
    }else{
        IR::sendLow();
    }
}

void IR::sendStartBit(){
    TCCR2A |= (1 << COM2B1);
    _delay_us(TIME_START);

    TCCR2A &= ~(1 << COM2B1);
    _delay_us(TIME_LOW);
}

void IR::sendHigh(){
    TCCR2A |= (1 << COM2B1);
    _delay_us(TIME_1);

    TCCR2A &= ~(1 << COM2B1);
    _delay_us(TIME_LOW);
}

void IR::sendLow(){
    TCCR2A |= (1 << COM2B1);
    _delay_us(TIME_0);

    TCCR2A &= ~(1 << COM2B1);
    _delay_us(TIME_LOW);
}