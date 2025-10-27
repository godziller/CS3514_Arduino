//J.P.M Nov 2020
// Bit Selection

#define BIT_TO_SELECT 6

void setup() {
  DDRD  = B11111111; //Set all port pin to output
}

void loop() {
  PORTD = 0xff; //set all pins HIGH
  delay(1000);
  PORTD &= ~(1<<BIT_TO_SELECT); // set BIT_TO_SELECT LOW
  delay(1000);
  PORTD = 0x00; //set all pins LOW
  delay(5000);
  PORTD |= (1<<BIT_TO_SELECT);  //set BIT_TO_SELECT HIGH
  delay (1000);
}

