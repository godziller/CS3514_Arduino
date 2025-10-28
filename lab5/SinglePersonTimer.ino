// --- PIN DEFINITIONS ---
#define NumberOfLEDS 6
#define BUTTON_BIT PD7    // Using D7 for the input switch
#define ASSERT(expr) if (!(expr)) {printf("The Following Failed: %s\n", #expr); exit(1);}

// Going to use 0.5sec for each LED to turn off.
#define FIXED_COUNTDOWN_DELAY 500 

// Need a mask for  LEDS D8-D13 - which are on PORTB
// Could just use B00111111 which maps to the PINS/LEDS
// But try and make code nicer with bit shift and OR operator
// PB0 = D8, PB1 = D9, ...PB5=D13
#define LED_MASK ((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5))
 

// Variable for timing reaction.
// Using big numbers here because we don't know how slow the user is.
// Time moves forward, so always positive.
unsigned long startTime = 0;
unsigned long endTime = 0;
unsigned long reactionTime = 0;

void setup() {
  // Using ASSERT from last week's solutoin to ensure working with correct
  // number of bits.
  ASSERT(NumberOfLEDS <= 6);
  
  // Set all my LED PINS as output.
  DDRB |= LED_MASK; 
  
  // And we turn them all off at the start.
  PORTB &= ~LED_MASK; 

  // Now set the Button/Switch PIN to an input
  DDRD &= ~(1 << BUTTON_BIT); 
  // and set it as pull up, so we declutter board not needing resistor
  PORTD |= (1 << BUTTON_BIT); 

  // Going to use Serial out to see what's going on.
  Serial.begin(9600);
  Serial.println("--- 6-LED Reaction Timer Ready ---");
}

void loop() {
  
  // And we're OFF to the races!!
  Serial.println("\nPress the button (D7) to start the countdown.");
  
  // Wait for the starting gun!!
  while (PIND & (1 << BUTTON_BIT)) { 
      // Wait forever for button pressed to start game.
  }
  
  // Using our LED mask, turn all LEDs on
  PORTB |= LED_MASK; 
  

  Serial.println("Countdown started");
  
  // 2. Countdown Sequence (Depleting: D8/PB0 is turned OFF first)
  for (int i = 0; i < NumberOfLEDS; i++) {
    
    delay(FIXED_COUNTDOWN_DELAY); 
    
    // Walk down the LEDs and turn each off in theloop
    PORTB &= ~(1 << i); 
    
  } // Loop finishes So now the timer /reaction starts.
  
  // Immediately after the last LED turns OFF, start the timer
  startTime = micros(); 
  // Use serial out to let us know where we are too.
  Serial.println("GO! (Last LED off - PRESS NOW!)");

  // Loop while the button is UNPRESSED (PIND bit 7 is HIGH)
  while (PIND & (1 << BUTTON_BIT)) {
    // Going to sit looping around here doing
    // nothing until button pressed.
  }

  // Ok button pressed, do calculations...
  endTime = micros();
  reactionTime = endTime - startTime;

  Serial.println("--- Reaction Complete ---");
  // Convert microseconds to milliseconds
  Serial.print("Reaction Time: ");
  Serial.print(reactionTime / 1000.0); 
  Serial.println(" ms"); 

  // Wait 5 seconds before allowing a new game
  delay(5000); 
}
