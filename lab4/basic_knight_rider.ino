#define LDR_PIN A0           // Analog pin for LDR

#define LDR_MIN 0            // LDR value in dark
#define LDR_MAX 1023         // LDR value in bright light

#define DELAY_MIN 50         // Fastest speed (dark)
#define DELAY_MAX 500        // Slowest speed (bright)

// Macro to calculate speed based on LDR input
#define LDR_TO_DELAY(x) map((x), LDR_MIN, LDR_MAX, DELAY_MIN, DELAY_MAX)

#define LED_COUNT 6          // Total number of LEDs

// LED pins: PD2–PD7 (Arduino digital pins D2–D7)
const uint8_t ledPins[LED_COUNT] = {2, 3, 4, 5, 6, 7};

void setup() {
  // Set LED pins as outputs using DDRD
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    DDRD |= (1 << ledPins[i]);
  }
}

void loop() {
  int lightLevel = analogRead(LDR_PIN);
  int delayTime = LDR_TO_DELAY(lightLevel);

  static int pos = 0;   // Current LED position (0–5)
  static int dir = 1;   // Direction: 1 = forward, -1 = backward

  // Light the current LED
  PORTD = (1 << ledPins[pos]);
  delay(delayTime);

  // Update position and reverse direction if needed
  pos += dir;
  dir = (pos == 0 || pos == (LED_COUNT - 1)) ? -dir : dir;
}
