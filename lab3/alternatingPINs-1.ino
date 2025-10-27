#define RED 0
#define GREEN 1

void setup() {
  DDRB = 0x03; //Pins 8 (RED) and 9 (GREEN) set as outputs
  DDRD = 0x0; // Pins 0..7 set as inputs
}

void loop() {
byte notPIND;
  (PIND & PIND<<1) ? PORTB |= 1<<RED : PORTB &= ~(1<<RED);
  notPIND = ~PIND;
  (notPIND & notPIND<<1) ? PORTB |= 1<<GREEN : PORTB &= ~(1<<GREEN);
}
