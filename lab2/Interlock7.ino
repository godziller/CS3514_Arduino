// Interlock on PORTD

#define S1 4
#define S2 5
#define L1 6
#define L2 7

#define SW_PRESSED(s) (!(PIND & (1<<s)))
#define TOGGLE(b)     (PORTD ^= (1<<b))
#define SET(b)        (PORTD |= (1<<b))
#define RESET(b)      (PORTD &= ~(1<<b))
#define IS_SET(b)     (PIND & (1<< b))


void setup() {
  DDRD  |= B11000000; // set pins 6 and 7 to be output, all other input
  PORTD |= B00110000; // Write a 1 to pins 4 and 5 to activate the pullups
}

void loop() {
  SW_PRESSED(S1) ? TOGGLE(L1), delay(200), 0 
                 : 0 ; // The 0s are inserted so that the types of seconds and third operands are the same
  IS_SET(L1) ? SW_PRESSED(S2) ? SET(L2) 
                              : RESET(L2)
              : RESET(L2);
}
