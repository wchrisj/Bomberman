#include "libraries/IRlib/IR.hpp"

int main(){
    uint8_t khz = 38;

    IRsend::test;

    IRsend sender = IRsend(khz);

    //IRsend::IRsend sender(khz);

    sender.send(0b00000000, 8);
    while(1){
        
    }

    return 0;
}