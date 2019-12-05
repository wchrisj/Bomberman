#include "Homepage.h"
#include "libraries/IRlib/IR.h"
#include "libraries/Nunchuk/nunchuk.h"

Adafruit_ILI9341 AdaHome = Adafruit_ILI9341(TFT_CS, TFT_DC);

IR ir = IR(3, 38);
NunchukInput* nunchuk1 = NunchukInput::getInstance();

Homepage::Homepage() {
    AdaHome.begin();
}

void Homepage::HomepageText(){                               // Het maken van het Start scherm
  AdaHome.fillScreen(ILI9341_GREENYELLOW);
  AdaHome.setCursor(00, 60);
  AdaHome.setTextColor(ILI9341_WHITE);  AdaHome.setTextSize(2);
  AdaHome.print("Druk 'Z' in ");
  AdaHome.setCursor(20, 100);
  AdaHome.setTextColor(ILI9341_BLUE);  AdaHome.setTextSize(3);
  AdaHome.print("Even geduld");
  AdaHome.setCursor(70, 140);
  AdaHome.setTextColor(ILI9341_RED);  AdaHome.setTextSize(3);
  AdaHome.print("A.U.B");

  //AdaHome.end();

}

void Homepage::HomepageText2(){
 AdaHome.fillScreen(ILI9341_BLACK);
  AdaHome.setCursor(00, 60);
  AdaHome.setTextColor(ILI9341_WHITE);  AdaHome.setTextSize(2);
  AdaHome.print("Druk 'Z' in ");
  AdaHome.setCursor(20, 100);
  AdaHome.setTextColor(ILI9341_BLUE);  AdaHome.setTextSize(3);
  AdaHome.print("Even geduld");
  AdaHome.setCursor(70, 140);
  AdaHome.setTextColor(ILI9341_RED);  AdaHome.setTextSize(3);
  AdaHome.print("A.U.B");

}


/*void Homepage::StartSending(){
  while (1)
  {
    //Knop Z is ingedrukt
    nunchuk1->getInstance();
    Serial.println(nunchuk1->status.Z);
	 if (nunchuk1->status.Z == 1) {  //test met waarde 0 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		Serial.println("Z knop werkt");  //print Z op serial terminal
    ir.send(0b01,0,0);    // Zend signaal 01  
    ir.enableReceiver();  //Zet de ontvanger aan
    Serial.println("informatie verzonden");
   }
   else {
     Serial.println("Noooooo");
   }

   delay(10);
}*/

