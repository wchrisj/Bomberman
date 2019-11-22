#include "IR.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

IRsettings_t IRsettings;
irparams_t irparams;
/*
    Constructor
    stelt de zendpin in en stelt de frequent van de timer op deze pin in
*/
IR::IR(uint8_t receivePin, uint8_t kHz){
    //Maak sender en receiver aan
    //Vul IRsettings

    IRsettings.receivePin = receivePin; // Zet de receivePin
    IRsettings.frequency = kHz;
    IRsettings.sendPin = (1 << PIND3); //Zet de zendpin als pin D3, deze hoort bij timer 2
    setupSendTimer(kHz);
}

void IR::enableReceiver(){
    cli(); // Zet even alle interrupt uit zodat dit goed ingesteld kan worden

    // Zet de ontvang pin goed
    DDRB &= ~(1 << IRsettings.receivePin);
    PORTB &= ~(1 << IRsettings.receivePin);

    TCCR2A = (1 << WGM21);  // Zet de timer op CTC mode
    TCCR2B = (1 << CS21);   // Zet de prescaler op 8
    OCR2A  = TIMER_COUNT_TOP / 8;
    TCNT2  = 0;

    TIMSK2 = (1 << OCIE2A); // Zet timer interrupt aan

    sei();  // Zet de interrupt weer aan

    IR::resumeReceiver(); // Ga data inlezen
}

uint8_t IR::decode(){
    if (irparams.receiveState != STOP){  //Dit check oP?
        IR::resumeReceiver(); // Ga nieuwe data inlezen
        return 0;
    }

    uint8_t dataBits = 0;
    uint16_t data = 0;
    for (int i = 1; i < irparams.rawlen; i++) { //Helft van de getallen zijn verkeerd.
        if (!(i & 1)) { //We zijn alleen geinteresseerd in tijd dat het signaal LOW is
            if (i == 1) {
                //Start bit
            }else if (irparams.rawbuf[i] * 50 <= MAX_ZERO_VALUE) {
                dataBits++;
            }else if (irparams.rawbuf[i] * 50 > MAX_ZERO_VALUE) {
                data += (1 << dataBits);
                dataBits++;
            }
        }
    }
    results.data = data;
    results.dataBits = dataBits;
    return 1;
}

void IR::resumeReceiver(){
    irparams.receiveState = IDLE;
    irparams.rawlen = 0;
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
    IR::setupSendTimer(IRsettings.frequency);

    //Zend start bit
    IR::sendStartBit();

    //Zend data en bereken paritybit
    uint8_t highBits = 0;
    for(uint8_t i = 0; i < len; i++){
        if(data & (1 << i)){
            IR::sendHigh();
            highBits++;
        }else{
            IR::sendLow();
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


//Timer interrupt voor het opvangen van IR signalen
ISR (TIMER2_COMPA_vect){
    uint8_t  irdata = (PINB & (1 << IRsettings.receivePin)) >> IRsettings.receivePin; // Lees de data in van de ontvang pin, 0 -> signaal is HIGH, 1 -> signaal is LOW

    irparams.timer++;  // Zet de timer 1 hoger, voor het meten van de tijd dat het signaal LOW is
    if (irparams.rawlen >= RAWBUF)  irparams.receiveState = OVERFLOW ;  // Past de nieuwe data er niet meer bij -> Zet de status op OVERFLOW

    switch(irparams.receiveState){
        case IDLE: // Is de ontvanger nergens mee bezig
            if (irdata == HIGH) { // Is er een verandering in het signaal
                if (irparams.timer < GAP_TICKS)  {  // Is het signaal lang genoeg LOW geweest -> is dit geen ruis?
                    irparams.timer = 0;                                 // Reset de timer
                } else {
                    irparams.rawlen++;                                      // Zet de lengte van de buffer +1
                    irparams.overflow                = 0;                   // Zet overflow op false
                    irparams.rawlen                  = 0;                   // Begin opnieuw met data in het buffer te plaatsen
                    irparams.rawbuf[irparams.rawlen] = irparams.timer;  // Sla de tijd op in het buffer
                    irparams.timer                   = 0;                   // Start de timer opnieuw
                    irparams.receiveState            = MARK;                // Zet de status op het meten van signaal HIGH
                }
            }
            break;
        case MARK: // Wordt de tijd dat het signaal HIGH is gemeten
            if (irdata == LOW) {   // Is het signaal niet meer HIGH -> sla dan de tijd op
                irparams.rawbuf[irparams.rawlen++] = irparams.timer;    // Sla de tijd op
                irparams.timer                     = 0;                     // Reset de timer zodat een nieuwe meting gedaan kan worden
                irparams.receiveState              = SPACE;                 // Begin de signaal LOW meting
            }
            break;
        case SPACE: // Wordt de tijd dat het signaal HIGH is gemeten
            if (irdata == HIGH) {  // Is het signaal niet meer LOW -> sla dan de tijd op
                irparams.rawbuf[irparams.rawlen++] = irparams.timer;    // Sla de tijd op
                irparams.timer                     = 0;                     // Reset de timer zodat een nieuwe meting gedaan kan worden
                irparams.receiveState              = MARK;                  // Begin de signaal HIGH meting
            } else if (irparams.timer > GAP_TICKS) {  // Is het signaal lang LOW, dan wordt er geen data meer verstuurd -> zet de status op stop
                irparams.receiveState = STOP;                               // Zet de status op STOP
            }
            break;
        case STOP:  // Is het signaal ontvangen
            if (irdata == HIGH){ // Is er verandering in het signaal
                irparams.timer = 0 ;  // Reset de timer
            }
            break;
        case OVERFLOW:  // Is het buffer vol?
            irparams.overflow = 1;        // Zet overflow op true
            irparams.receiveState = STOP; // Zet de status op STOP
            break;
    }
}
