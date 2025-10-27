//J.P.M Nov 2020
// Toggle bit pattern

#define BITS_TO_TOGGLE 0x3c //00111100

void setup() {
  DDRD  = B11111111;
  PORTD = BITS_TO_TOGGLE;
}

void loop() {
  PORTD ^= BITS_TO_TOGGLE;
  delay(500);
}


