#define RED 0
#define GREEN 1

#define LED_ON(b)  (PORTB |= 1<<b)
#define LED_OFF(b) (PORTB &= ~(1<<b))

void setup() {
  DDRB = 0x03; //Pins 8 (RED) and 9 (GREEN) set as outputs
  DDRD = 0x0; // Pins 0..7 set as inputs
}

void loop() {


  (PIND & PIND<<1) ? LED_ON(RED) : LED_OFF(RED);
  ((uint8_t)(~PIND) & ((uint8_t)(~PIND)<<1)) ? LED_ON(GREEN) : LED_OFF(GREEN);
}
