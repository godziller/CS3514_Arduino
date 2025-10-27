//J.P.M Nov 2020
// Transmit a byte serially on a pin

#define TX_PIN 7    //Transmission Pin 
#define DIRECTION 0 // 1 for msb to lsb; 0 for lsb to msb
#define BIT_RATE 1000

byte outBuffer = 0x8A; //Value to transmit
byte mask;

void setup() {
  pinMode(TX_PIN, OUTPUT);

  if (DIRECTION){
    for (mask=0x80; mask > 0; mask >>=1){
      (outBuffer & mask) ? digitalWrite(TX_PIN, HIGH) : digitalWrite(TX_PIN, LOW);
      delay(BIT_RATE);
    }
  }else{
    for (mask=1; mask; mask <<=1){
      (outBuffer & mask) ? digitalWrite(TX_PIN, HIGH) : digitalWrite(TX_PIN, LOW);
      delay(BIT_RATE);
    }
   
  }
  digitalWrite(TX_PIN, LOW);
}

void loop() {}
