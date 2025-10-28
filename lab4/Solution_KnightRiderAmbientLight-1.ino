//J.P.M Nov 2020
// Bit Set/Reset

#define NumberOfLEDS 6
#define ASSERT(expr) if (!(expr)) {printf("The Following Failed: %s\n", #expr); exit(1);}

#define MIN_DELAY 5   // time in ms
#define MAX_DELAY 500 // time in ms

#define MIN_SERSOR_VAL 90  //value when darkest
#define MAX_SERSOR_VAL 800 //value when brightest


void setup() {
  ASSERT(NumberOfLEDS <=8);
  DDRB = B11111111;
  PORTB = 0xff;
}

void loop() {
  int i;
  for (i=0;i<NumberOfLEDS;i++){
    PORTB |= (1<<i); //set each bit in turn. From Bit 0 to Bit 5
    delay(map(analogRead(A0),MIN_SERSOR_VAL,MAX_SERSOR_VAL,MAX_DELAY,MIN_DELAY));
  }
  for (i=0;i<NumberOfLEDS;i++){
    PORTB &= ~(1<<i);//reset each bit in turn. From Bit 0 to Bit 5
    delay(map(analogRead(A0),MIN_SERSOR_VAL,MAX_SERSOR_VAL,MAX_DELAY,MIN_DELAY));
  }
    for (i=NumberOfLEDS-1;i>=0;i--){
    PORTB |= (1<<i); //set each bit in turn. From Bit 5 to Bit 0
    delay(map(analogRead(A0),MIN_SERSOR_VAL,MAX_SERSOR_VAL,MAX_DELAY,MIN_DELAY));
  }
  for (i=NumberOfLEDS-1;i>=0;i--){
    PORTB &= ~(1<<i);//reset each bit in turn. From Bit 5 to Bit 0
    delay(map(analogRead(A0),MIN_SERSOR_VAL,MAX_SERSOR_VAL,MAX_DELAY,MIN_DELAY));
  }
}
