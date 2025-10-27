

#define S1 4
#define S2 5
#define L1 6
#define L2 7
#define OFF 0
#define ON 1

char L1State;
byte  portD;

void setup() {
  DDRD  |= B11000000; // set pins 6 and 7 to be output, all other input
  PORTD |= B00110000; // Write a 1 to pins 4 and 5 to activate the pullups
  L1State = OFF; 
}

void loop() {
  portD = PIND;
  //if S1 is pressed toggle L1State and L1
  if(!(portD & (1<<S1))){
    (L1State = !L1State) ?  PORTD |= (1<<L1) : PORTD &= ~(1<<L1);
    delay(200);
  } 
  L1State ? (!(portD & (1<<S2))) ? PORTD |= (1<<L2) : PORTD &= ~(1<<L2) : PORTD &= ~(1<<L2);
 //if the L1 is on, turn L2 on depending on whether S2 is pressed. If L1 is off make sure L2 is also off.
}
