

#define S1 4
#define S2 5
#define L1 6
#define L2 7
#define OFF 0
#define ON 1

char L1State;

void setup() {
  DDRD  |= B11000000; // set pins 6 and 7 to be output, all other input
  PORTD |= B00110000; // Write a 1 to pins 4 and 5 to activate the pullups
  L1State = OFF; 
}

void loop() {
  if(!digitalRead(S1)){  //The normally HIGH input has been pulled LOW - S1 is pressed
      digitalWrite(L1, L1State = !L1State); //toggle L1 and record that in L1state
      delay(200);  //debounce
  } 
  if (L1State)  // L1 is on
    if(!digitalRead(S2)) //The normally HIGH input has been pulled LOW - S2 is pressed
      digitalWrite(L2, HIGH);
    else
      digitalWrite(L2, LOW);
  else
    digitalWrite(L2, LOW); //Extinguish L2 if L1 is turned off when L2 is on
}

