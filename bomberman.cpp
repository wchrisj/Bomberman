#include "libraries/IRlib/IR.h"

#include <avr/io.h>
#include <util/delay.h>

int main(){
    uint8_t khz = 38;

    IR sender = IR(3, khz);

    while(1){
    sender.send(0xAA, 8);
    _delay_ms(500);
    }

    return 0;
}