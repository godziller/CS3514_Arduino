

#define S1 4
#define S2 5
#define L1 6
#define L2 7

void setup() {
  DDRD  |= B11000000; // set pins 6 and 7 to be output, all other input
  PORTD |= B00110000; // Write a 1 to pins 4 and 5 to activate the pullups
}

void loop() {
  //if S1 is pressed toggle L1
  (!(PIND & (1<<S1))) ? PORTD ^= (1<<L1), delay(200), 1 : 1 ; // The 1s are inserted so that the types of seconds and third operands are the same
  (PIND & (1<< L1)) ? ((!(PIND & (1<<S2))) ? PORTD |= (1<<L2) : PORTD &= ~(1<<L2)) : PORTD &= ~(1<<L2);
 //if the L1 is on, turn L2 on if S2 is pressed. If L1 is off make sure L2 is also off.
}
