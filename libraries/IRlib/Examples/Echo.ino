#include "libraries/IRlib/IR.h"

#include <avr/io.h>
#include <util/delay.h>

int main(){
    IR ir = IR(3, 38);

    ir.enableReceiver();

    while(1){
        if(ir.decode()){
            Serial.print(ir.results.identifier, BIN);
            Serial.print(" ");
            Serial.print(ir.results.data, HEX);
            Serial.print(" ");
            Serial.println(ir.results.dataBits);
            ir.send(0b01, ir.results.data, ir.results.dataBits);
            // ir.send(0x11, 8);
            ir.enableReceiver();
            ir.resumeReceiver();
        }
        // ir.send(0x82, 8);
        // ir.enableReceiver();
        _delay_ms(100);
    }

    return 0;
}