#define S1 4
#define S2 5
#define L1 6
#define L2 7

int L1State;

void setup() {
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);
  L1State = 0; //indicating that L1 is LOW
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
}

void loop() {
  if(!digitalRead(S1)){  //The normally HIGH input has been pulled LOW - S1 is pressed
      digitalWrite(L1, L1State = !L1State); //toggle L1 and record that in L1state
      delay(200);  //debounce
  } 
  if (L1State)  // L1 is on
    if(!digitalRead(S2))
      digitalWrite(L2, HIGH);
    else
      digitalWrite(L2, LOW);
  else
    digitalWrite(L2, LOW); //Extinguish L2 if L1 turn off when L2 is on
}

