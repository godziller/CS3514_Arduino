// --- PORTB MACROS (Pins D8-D13) ---
// Choosing to use the same port for button and led - PORTB.
// But naming carefully with _B, so coder understands dealing with 
// PORTB. 
#define SET_BIT_B(BIT) (PORTB |= (1 << BIT))
#define CLR_BIT_B(BIT) (PORTB &= ~(1 << BIT))
#define READ_BIT_B(BIT) (PINB & (1 << BIT))

// --- PORTD MACROS (Pins D0-D7 - NEW FOR BUTTONS) ---
// Sets a bit in PORTD (Used to enable PULLUP if pin is input)
#define SET_BIT_D(BIT) (PORTD |= (1 << BIT))
// Clears a bit in PORTD low 
#define CLR_BIT_D(BIT) (DDRD &= ~(1 << BIT)) 
// Reads the state of a specific pin
#define READ_BIT_D(BIT) (PIND & (1 << BIT))


// --- PIN DEFINITIONS ---
// Reflect how we wire our solution - Pin8 for LED, pin 5 for Start, pin 6 for Stop.
#define START_LED_BIT PB0   // Pin 8 (PB0) is the start signal LED.

// Pins D5/D6 are used for buttons and belong to PORTD
#define START_BUTTON_BIT PD5 // Pin 5 (PD5) is the button to START the game.
#define STOP_BUTTON_BIT PD6  // Pin 6 (PD6) is the button to STOP/calc reaction.

// Assignment Constraints for random number range
// Used for random number/ wait time generation.
#define WAIT_MIN 0       // Minimum random wait time (0 seconds)
#define WAIT_MAX 10000   // Maximum random wait time (10 seconds)


// Variable for timing reaction.
// Using big numbers here because we don't know how slow the user is.
unsigned long startTime = 0;
unsigned long endTime = 0;
unsigned long reactionTime = 0;
unsigned long randomWaitTime = 0; // The calculated delay before GO signal

// Max out initial best time so the first game run will be the best (hopefully!).
unsigned long bestTime = 0xFFFFFFFF; 
// Running counter for multiple game plays - needs to be big -i.e. unsigned long
unsigned long totalReactionTime = 0;

// Record the number of games played to help with the average calc
int gameCount = 0;
// used for average reaction time
float averageReactionTime = 0.0;


// Using a fucntion to keep code modular and readable.
// this shoudl take in the current game result and update the 
// game variables above, then print the running results...
void reportResults(unsigned long currentReactionTime);

// Added prototype for scoreboard reset function
void resetScoreboard();


void setup() {
  // Set up serial so we can see what's on.
  Serial.begin(9900); 
  
  // Arduino random function needs setup, so I need to initialize it
  // here for use later. Using board time as the seed 'cos I can't use 
  // the AnalogRead() recommended in arduino docs due to lab guidance.
  randomSeed(micros()); 
  
  Serial.println("Game ON!");

  // Set up input and output configuration - LED->out, Button->in
  
  // LED (PORTB) Setup: Pin D8 (PB0) is OUTPUT
  DDRB |= (1 << START_LED_BIT); // D8 (LED) is OUTPUT
  
  // BUTTONS (PORTD) Setup
  
  // Explicitly clear the DDRD bits for D5 and D6 to ensure they are INPUTS
  DDRD &= ~((1 << START_BUTTON_BIT) | (1 << STOP_BUTTON_BIT));

  // Need to turn off LED and enable button for PULLUP
  CLR_BIT_B(START_LED_BIT);  // D8 LED OFF
  
  // Enable PULLUP for both buttons by setting the PORTD bits
  SET_BIT_D(START_BUTTON_BIT); // D5 Enable PULLUP
  SET_BIT_D(STOP_BUTTON_BIT);  // D6 Enable PULLUP
}


void loop() {
  
  // We begin with a print to screen
  Serial.println("\nPress Button to start game!");
  
  // A loop waiting for user to press button (Start Button - D5).
  while (READ_BIT_D(START_BUTTON_BIT)) {
    // Using debounce because especially this game likely to happen.
    delay(50); 
  }
  
  // Need this also because the player may have kept the button
  // depressed. So this waits for the button to be released.
  
  while (!READ_BIT_D(START_BUTTON_BIT)) { 
    // Using debounce because especially this game likely to happen.
    delay(50); 
  }

  Serial.println("Ready... Keep eye on LED!!!.");

  // Generate a random wait/backoff time and get a start time
  // recorded for the game.
  randomWaitTime = random(WAIT_MIN, WAIT_MAX + 1); 
  unsigned long randomStartTime = millis();

  // Countdown loop, take current time from the recorded start time
  // and see if it is less than what we wait. If true, keep waiting.
  // otherwise move on
  while (millis() - randomStartTime < randomWaitTime) {
    
    // Is while we are waiting here for the go signal
    // that a player may falsely press the button (Stop Button - D6).
    // if they do that we need to clear out all the game stats data
    // they lose big time!!
    if (!READ_BIT_D(STOP_BUTTON_BIT)) {
      // Ooops - too soon!!
      Serial.println("\n!!! TOO IMPATIENT!!  !!!");
      Serial.println("Looser!! All your game data in the bin!!.");
      
      // Reset all scoring variables as required by the update
      resetScoreboard();
      
      // Need this also because the player may have kept the button
      // depressed. So this waits for the button to be released.
      while (!READ_BIT_D(STOP_BUTTON_BIT)) {
          // Wait for release
      } 
      // random delay to exit
      delay(5000); 
      return; // Break out of this loop and go back to main loop.
    }
    // The code simply waits here for the random time to pass.
  }

  // Game ON! light LED to signal to player to react.
  SET_BIT_B(START_LED_BIT); 
  
  // Immediately start the timer to time the user reaction.
  startTime = micros(); 
  Serial.println("GO! GO! GO!, PRESS STOP BUTTON (D6)!!");
  
  
  // Loop while the button is UNPRESSED (Stop Button - D6 is HIGH)
  while (READ_BIT_D(STOP_BUTTON_BIT)) {
    // Breaking out of this signals Player has pressed button.
  }

  // Record the result.
  endTime = micros();
  reactionTime = endTime - startTime;

  // Turn the start signal LED OFF in prep for new game.
  CLR_BIT_B(START_LED_BIT); 

  // hand over to function to do the printing...
  reportResults(reactionTime); 

  // Hold off for a bit before starting new game.
  delay(5000); 
}


// Takes in current score, updates the game count.
// check for if this is the best time and updates.
// calcs the average...
void reportResults(unsigned long currentReactionTime) {
  
  // Update Game Count - need for proper average calc.
  gameCount++;
  
  // same here for average calc
  totalReactionTime += currentReactionTime;

  // Running Average calc
  averageReactionTime = (float)totalReactionTime / gameCount;
  
  // Record best time?? 
  if (currentReactionTime < bestTime) {
    bestTime = currentReactionTime;
  }
  
  // Now to display part....
  
  // Banner
  Serial.println("GAME OVER");
  
  // Display random wait time
  Serial.print("Random Wait Time was: ");
  // Note: randomWaitTime is a global variable
  Serial.print(randomWaitTime); 
  Serial.println(" ms");
  
  // Convert microseconds to milliseconds for display
  // ealier use of millis were quite untidy, we we do a simple downshift
  float currentReactionTimeMS = currentReactionTime / 1000.0;
  float bestTimeMS = bestTime / 1000.0;
  float averageTimeMS = averageReactionTime / 1000.0;
  
  // Convert microseconds to milliseconds
  Serial.print("REACTION TIME: ");
  Serial.print(currentReactionTimeMS, 2); 
  Serial.println(" ms"); 

  Serial.println("--- RESULTS ---");
  Serial.print("BEST TIME (Total Games: ");
  Serial.print(gameCount);
  Serial.print("): ");
  Serial.print(bestTimeMS, 2);
  Serial.println(" ms");
  
  Serial.print("AVERAGE REACTION TIME: ");
  Serial.print(averageTimeMS, 2);
  Serial.println(" ms");
}

// New function definition for resetting scoring variables
void resetScoreboard() {
    // Max out initial best time so the next game run will be the best (hopefully!).
    bestTime = 0xFFFFFFFF; 
    // Running counter for multiple game plays - reset to 0
    totalReactionTime = 0;
    // Record the number of games played - reset to 0
    gameCount = 0;
    // used for average reaction time - reset to 0.0
    averageReactionTime = 0.0;
}
