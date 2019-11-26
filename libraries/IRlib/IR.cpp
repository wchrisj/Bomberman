#include "IR.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

IRsettings_t IRsettings;
irparams_t irparams;
/*
    Constructor
    stelt de zendpin en ontvangpin in en stelt de frequent van de timer op de zendpin in
*/
IR::IR(uint8_t receivePin, uint8_t kHz){
    IRsettings.receivePin = receivePin; // Zet de receivePin
    IRsettings.frequency = kHz;         // Stel de frequentie in
    IRsettings.sendPin = (1 << PIND3); //Zet de zendpin als pin D3, deze hoort bij timer 2
    setupSendTimer(kHz);
}

/*
    Deze functie stelt timer 2 in voor het ontvangen van data en zet de receivepin op de goede stand
*/
void IR::enableReceiver(){
    cli(); // Zet even alle interrupt uit zodat dit goed ingesteld kan worden

    // Zet de ontvang pin goed
    DDRB &= ~(1 << IRsettings.receivePin);
    PORTB &= ~(1 << IRsettings.receivePin);

    TCCR2A |= (1 << WGM21);  // Zet de timer op CTC mode
    TCCR2A &= ~(1 << WGM20);
    TCCR2B &= ~(1 << WGM22);

    TCCR2B &= ~((1 << CS20) | (1 << CS22)); 
    TCCR2B |= (1 << CS21);   // Zet de prescaler op 8
    OCR2A  = TIMER_COUNT_TOP / 8;
    TCNT2  = 0;

    TIMSK2 |= (1 << OCIE2A); // Zet timer interrupt aan

    sei();  // Zet de interrupt weer aan

    IR::resumeReceiver(); // Ga data inlezen
}

/*
    Deze functie vertaal de intervallen uit irparams.rawbuf en zet de data in results.data en results.bits
    return 0 als er niet gedecode kan worden
    return 1 als het decoden gelukt is
*/
uint8_t IR::decode(){
    if (irparams.receiveState != STOP){  //Dit check oP?
        IR::resumeReceiver(); // Ga nieuwe data inlezen
        return 0;
    }

    uint8_t dataBits = 0;
    uint16_t data = 0;
    uint8_t highBitsCounter = 0;
    for (int i = 1; i < irparams.rawlen; i++) { //Helft van de getallen zijn verkeerd.
        if (!(i & 1)) { //We zijn alleen geinteresseerd in tijd dat het signaal LOW is
            if (i == 1) {
                //Start bit
            }else if (irparams.rawbuf[i] * 50 <= MAX_ZERO_VALUE) {
                dataBits++;
            }else if (irparams.rawbuf[i] * 50 > MAX_ZERO_VALUE) {
                data += (1 << dataBits);
                dataBits++;
                highBitsCounter++;
            }
        }
    }

    dataBits--; // Het laaste bit is het parity bit en moet dus niet meetellen

    //Kijk is het paritybit goed is, en de data dus goed is overgekomen
    if(highBitsCounter & 1){ // Laaste bit is 1 dus oneven dus de data is niet goed
        IR::resumeReceiver(); // Ga nieuwe data inlezen
        return 0;
    }

    results.parityCheck = 1;
    results.data = data & ~(1 << dataBits); // Het paritybit hoort niet bij de data
    results.data = results.data >> 2; // Eerste 2 bits zijn identifiers en geen data
    results.identifier = data & 0x03;
    results.dataBits = dataBits-2; // Zonder parity bit
    return 1;
}

/*
    Deze functie zorgt ervoor dat er nieuwe data ingelezen gaat worden
*/
void IR::resumeReceiver(){
    irparams.receiveState = IDLE;
    irparams.rawlen = 0;
}

/*
    Deze functie stelt timer 2 in voor het versturen van informatie
*/
void IR::setupSendTimer(uint8_t kHz){
    //Zet de interrupts tijdelijk uit
    cli();

    //Stel de timer in 
    //Fast PWM, top = OCR2A
    TCCR2A |= (1 << WGM20);
    TCCR2A &= ~(1 << WGM21); 
    TCCR2B |= (1 << WGM22);

    TCCR2B |= (1 << CS20); //Geen prescaling
    TCCR2B &= ~((1 << CS21) | (1 << CS22));

    OCR2A = F_CPU / 2000 / kHz;
    OCR2B = (F_CPU / 2000 / kHz)/3;

    TIMSK2 = 0; // Geen timer interrupts

    // Zendpin instellen als output pin
    DDRD |= (1 << DDD3);
    PIND &= ~(1 << DDD3);

    //Zet de interrupts weer aan
    sei();
}

/*
    Deze functie verstuurt data met lengte len
    Aan het eind van data word een even parity bit toegevoegd
*/
void IR::send(uint8_t identifier, uint16_t data, uint8_t len){
    //Zet de timer goed
    IR::setupSendTimer(IRsettings.frequency);

    //Zend start bit
    IR::sendStartBit();

    //Zend data en bereken paritybit
    uint8_t highBits = 0;
    for(uint8_t i = 0; i < 2; i++){
        if(identifier & (1 << i)){
            IR::sendHigh();
            highBits++;
        }else{
            IR::sendLow();
        }
    }
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

/*
    Deze functie zorgt dat er een startbit verstuurt worden
*/
void IR::sendStartBit(){
    TCCR2A |= (1 << COM2B1);
    _delay_us(TIME_START);

    TCCR2A &= ~(1 << COM2B1);
    _delay_us(TIME_LOW);
}

/*
    Deze functie zorgt dat er een 1 verstuurt worden
*/
void IR::sendHigh(){
    TCCR2A |= (1 << COM2B1);
    _delay_us(TIME_1);

    TCCR2A &= ~(1 << COM2B1);
    _delay_us(TIME_LOW);
}

/*
    Deze functie zorgt dat er een 0 verstuurt worden
*/
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
                    irparams.rawbuf[irparams.rawlen] = irparams.timer;      // Sla de tijd op in het buffer
                    irparams.timer                   = 0;                   // Start de timer opnieuw
                    irparams.receiveState            = MARK;                // Zet de status op het meten van signaal HIGH
                }
            }
            break;
        case MARK: // Wordt de tijd dat het signaal HIGH is gemeten
            if (irdata == LOW) {   // Is het signaal niet meer HIGH -> sla dan de tijd op
                irparams.rawbuf[irparams.rawlen++] = irparams.timer;        // Sla de tijd op
                irparams.timer                     = 0;                     // Reset de timer zodat een nieuwe meting gedaan kan worden
                irparams.receiveState              = SPACE;                 // Begin de signaal LOW meting
            }
            break;
        case SPACE: // Wordt de tijd dat het signaal HIGH is gemeten
            if (irdata == HIGH) {  // Is het signaal niet meer LOW -> sla dan de tijd op
                irparams.rawbuf[irparams.rawlen++] = irparams.timer;        // Sla de tijd op
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
