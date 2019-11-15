#include "IR.h"

/*
    Constructor
    stelt de zendpin in en stelt de frequent van de timer op deze pin in
*/
IRreceive::IRreceive(uint8_t receivePin){
    IRsettings.sendPin = receivePin;
}

void IRreceive::enable(){

}