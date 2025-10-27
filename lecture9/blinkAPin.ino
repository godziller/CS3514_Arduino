//J.P.M Nov 2020
//Blink a Specific Pin in a Port

#define PIN_TO_BLINK 0x20 // 00100000, i.e., Pin 5 
#define PORT_CONFIGURATION 0x23 //00100011 Pins 0,1 and 5 are output 
#define BLINK_RATE 300

void setup() {
   DDRD  = PORT_CONFIGURATION; 
   PORTD = 0x00;
}

void loop() {
   PORTD ^= PIN_TO_BLINK;
   delay(BLINK_RATE);

}


