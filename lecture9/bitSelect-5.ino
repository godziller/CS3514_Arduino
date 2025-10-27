//J.P.M Nov 2020
// Bit Selection

#define BIT_TO_SELECT 6

void setup() {
  DDRD  = B11111111; //Set all port pin to output
}

void loop() {
  PORTD = 0xff; //set all pins HIGH
  delay(1000);
  // the ~ inverts so in this case you end up with 11011111
  //which then wiht an & filters and sets bit 6 to 0
  // Basically use &= coupled with ~ to set a bit low.
  PORTD &= ~(1<<BIT_TO_SELECT); // set BIT_TO_SELECT LOW
  delay(1000);
  PORTD = 0x00; //set all pins LOW
  delay(5000);

  // kinda opposite to earlier, here you are using the OR function.
  // to inside the () you get 00100000, then the |= just affects the target bit.
  PORTD |= (1<<BIT_TO_SELECT);  //set BIT_TO_SELECT HIGH
  delay (1000);
}


