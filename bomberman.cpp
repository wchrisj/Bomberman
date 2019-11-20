#include "libraries/IRlib/IR.h"

#include <avr/io.h>
#include <util/delay.h>

int main(){
    uint8_t khz = 38;

    IR sender = IR(3, khz);


    // DDRD |= (1 << DDD3);
    // PIND |= (1 << DDD3);
    while(1){
    sender.send(0xFF, 8);
    _delay_ms(500);
    }

    return 0;
}