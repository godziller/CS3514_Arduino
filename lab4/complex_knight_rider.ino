
// The design is the head/leader LED is full brighness (255), the the 2 followers
// are reduced in brighness, closest follower is 127 and 32 - a 1/2 and a 1/4 of the full intensity.
// Could choose other values, but picked these for simplicity.

// Trail brightness (0..255): head and its two neighbors
#define HEAD_PWM         255         // leader
#define FOLLOW1_PWM      127         // ±1 (≈ 1/2)
#define FOLLOW2_PWM      32          // ±2 (≈ 1/4)

// --- Ambient input (LDR on A0) controls BRIGHTNESS only ---------------------
#define LDR_PIN          A0
#define AMBIENT_MIN_RAW  0           // darkest possible reading
#define AMBIENT_MAX_RAW  1023        // brightest possible reading
#define BRIGHT_MIN       0          // dimmest overall brightness
#define BRIGHT_MAX       255         // brightest overall brightness

// Map raw ambient to overall brightness (0..255)
#define MAP_BRIGHTNESS(raw_analog_input) \
  ((uint8_t) map((long)(raw_analog_input), AMBIENT_MIN_RAW, AMBIENT_MAX_RAW, BRIGHT_MIN, BRIGHT_MAX))

// Frame pacing (fixed; lower = faster sweep)
#define FRAME_DWELL_MS   60

// Use Macros to create Small helpers
#define ARRAY_LEN(a)     (sizeof(a) / sizeof((a)[0]))
#define DIST(a,b)        ((a) >= (b) ? ((a) - (b)) : ((b) - (a)))  // Calc absolute distance because distance may be negative.

// Needed to find what PINs can support analog write - these are the only ones...
const uint8_t PINS[] = {3, 5, 6, 9, 10, 11};
const int LED_COUNT = sizeof(PINS) / sizeof(PINS[0]);

int pos = 0;   // current LED index (0..LED_COUNT-1)
int dir = 1;   // movement direction (+1 forward, -1 back)

void setup() {
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(PINS[i], OUTPUT);
    analogWrite(PINS[i], 0);   // start off
  }
}

void loop() {
  // Read ambient light (raw 0..1023) and map to overall brightness
  int raw_analog_input = analogRead(LDR_PIN);
  uint8_t mapped_brightness = MAP_BRIGHTNESS(raw_analog_input);  // 0..255 by default

  // Draw one frame
  for (int i = 0; i < LED_COUNT; i++) {
    int distance_from_head = DIST(i, pos);

    // Base brightness by distance from head
    uint8_t base_pwm = (distance_from_head == 0) ? HEAD_PWM
                       : (distance_from_head == 1) ? FOLLOW1_PWM
                       : (distance_from_head == 2) ? FOLLOW2_PWM
                       : 0;
    // Now scale base by ambient brightness based on mapped brighness
    uint8_t pwm_out = (base_pwm * mapped_brightness) / 255;

    analogWrite(PINS[i], pwm_out);   // 0..255 PWM
  }

  delay(FRAME_DWELL_MS);             // fixed pacing

  // Advance and bounce
  pos += dir;
  dir = (pos == 0 || pos == (LED_COUNT - 1)) ? -dir : dir;
}

