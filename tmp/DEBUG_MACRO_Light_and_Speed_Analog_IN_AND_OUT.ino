// Analog in program to influence analog out pin values.
#define DEBUG 1  // Set to 1 to enable debug prints, 0 to disable
#if DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_DELAY(x) delay(x)
  #define DEBUG_BEGIN() Serial.begin(9600)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_DELAY(x)
  #define DEBUG_BEGIN()
#endif
#define NumberOfLEDS 6
#define ASSERT(expr) if (!(expr)) {printf("The Following Failed: %s\n", #expr); exit(1);}
#define MIN_DELAY 5   // time in ms
#define MAX_DELAY 500 // time in ms
// Analog Input Sensor
#define MIN_SENSOR_VAL 90  //value when darkest
#define MAX_SENSOR_VAL 800 //value when brightest
// LED Analog output range
#define PWM_MIN = 0   // LED always OFF
#define PWM_MAX = 255 // LED mostly ON (appears fully bright)

// PWM-capable pins on Arduino Uno
int ledPins[NumberOfLEDS] = {3, 5, 6, 9, 10, 11};

void setup() {
  DEBUG_BEGIN();
  DEBUG_PRINT("Entering setup()\n");
  
  //Use ASSERT to ensure we have the right PIN config.
  ASSERT(NumberOfLEDS <=6);  // Only 6 PWM pins available
  
  // Set PWM pins as outputs using DDR registers with pin macros
  // Could have used a simpler loop and pinMode run on an array.
  // But here using faster DDR.
  // Because the PWM PINs are spread across 2 port families - D and B.
  // need to adjust each accordingly.
  DDRD |= (1<<PD3) | (1<<PD5) | (1<<PD6);  // Pins 3, 5, 6 are on PORTD
  DDRB |= (1<<PB1) | (1<<PB2) | (1<<PB3);  // Pins 9, 10, 11 are on PORTB
  
  DEBUG_PRINT("Exiting setup()\n");
}

// Using function that takes in a pointer to the values we want modified
// Note, analog read is done inside this function, just like last weeks solution
// but we could do it once every loop, which would then modify this function to 
// take a third 'sensorVal' argument that is per loop cycle.
// However inside the function I will only use analogRead once to ensure
// I'm synchronized between light and speed for the function call.
void readSensorValues(unsigned int *delayTime, unsigned int *pwmValue) {
  DEBUG_PRINT("  Entering readSensorValues()\n");
  
  int sensorVal = analogRead(A0);
  
  // Cast map() results to unsigned int to prevent overflow issues
  *delayTime = (unsigned int)map(sensorVal, MIN_SENSOR_VAL, MAX_SENSOR_VAL, MAX_DELAY, MIN_DELAY);
  *pwmValue = (unsigned int)map(sensorVal, MIN_SENSOR_VAL, MAX_SENSOR_VAL, PWM_MIN, PWM_MAX);
  
  // Constrain values to valid ranges as safety measure
  *delayTime = constrain(*delayTime, MIN_DELAY, MAX_DELAY);
  *pwmValue = constrain(*pwmValue, 0, 255);
  
  DEBUG_PRINT("  Sensor: ");
  DEBUG_PRINT(sensorVal);
  DEBUG_PRINT(", Delay: ");
  DEBUG_PRINT(*delayTime);
  DEBUG_PRINT(", PWM: ");
  DEBUG_PRINT(*pwmValue);
  DEBUG_PRINT("\n");
  
  DEBUG_PRINT("  Exiting readSensorValues()\n");
}

void loop() {
  DEBUG_PRINT("Entering loop()\n");
  
  int i;
  // these variables are going to be adjusted per the A0 input value.
  unsigned int delayTime, pwmValue;
  
  // Note, taking instruction from last week's solution, choosing to put 
  // the readSensorValues/analogRead inside each loop for accuracy.
  // could have extracted it outside this block if we wished.
  for (i=0;i<NumberOfLEDS;i++){
    readSensorValues(&delayTime, &pwmValue);
    analogWrite(ledPins[i], pwmValue); //turn on each LED with PWM
    delay(delayTime);
  }
  for (i=0;i<NumberOfLEDS;i++){
    readSensorValues(&delayTime, &pwmValue);
    analogWrite(ledPins[i], 0); //turn off each LED
    delay(delayTime);
  }
  for (i=NumberOfLEDS-1;i>=0;i--){
    readSensorValues(&delayTime, &pwmValue);
    analogWrite(ledPins[i], pwmValue); //turn on each LED with PWM
    delay(delayTime);
  }
  for (i=NumberOfLEDS-1;i>=0;i--){
    readSensorValues(&delayTime, &pwmValue);
    analogWrite(ledPins[i], 0); //turn off each LED
    delay(delayTime);
  }
  
  DEBUG_PRINT("Exiting loop()\n");
  DEBUG_PRINT("---\n");
  
  DEBUG_DELAY(1000);  // Slow down for debugging
}
