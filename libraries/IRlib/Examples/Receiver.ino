#include "libraries/IRlib/IR.h"

#include <avr/io.h>
#include <util/delay.h>

int main(){
    uint8_t khz = 56;

    IR ir = IR(3, khz);

    while(1){
    ir.send(0b10,0x10, 8);
    ir.enableReceiver();
    _delay_ms(500);

    if(ir.decode()){
        Serial.print(ir.results.identifier, BIN);
        Serial.print( " ");
        Serial.print(ir.results.data, HEX);
        Serial.print(" ");
        Serial.println(ir.results.dataBits);
        ir.resumeReceiver();

    }
    }

    return 0;
}