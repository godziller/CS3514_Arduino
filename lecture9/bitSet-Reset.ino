//J.P.M Nov 2020
// Bit Set/Reset

void setup() {
  DDRD  = B11111111;
  PORTD = 0x00;
}

void loop() {
  int i;

  for (i=0;i<=7;i++){
    PORTD |= (1<<i); //set each bit in turn
    delay(100);
  }
  for (i=0;i<=7;i++){
    PORTD &= ~(1<<i);//reset each bit in turn
    delay(100);
  }
}
