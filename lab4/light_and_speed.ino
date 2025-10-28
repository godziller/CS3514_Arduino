// ─────────────────────────────────────────────────────────────────────────────
// Knight Rider (PWM Trail) + Ambient (LDR on A0)
// LDR controls BOTH overall brightness and sweep speed.
// ─────────────────────────────────────────────────────────────────────────────

// Trail brightness (0..255): head and its two neighbors, reduced by 1/2 and 1/4
#define HEAD_PWM         255     // leader
#define FOLLOW1_PWM      127     // ±1 (≈ 1/2)
#define FOLLOW2_PWM      32      // ±2 (≈ 1/4)

// Ambient input (LDR on A0)
#define LDR_PIN          A0
#define AMBIENT_MIN_RAW  0       // darkest possible reading
#define AMBIENT_MAX_RAW  1023    // brightest possible reading

// LED Brightness mapping (overall scale)
#define BRIGHT_MIN       0       // dimmest overall brightness
#define BRIGHT_MAX       255     // brightest overall brightness

// Macro to help translate/scale raw analog input (ambient) to brightness output
#define MAP_BRIGHTNESS(raw) \
  ((uint8_t) map((long)(raw), AMBIENT_MIN_RAW, AMBIENT_MAX_RAW, BRIGHT_MIN, BRIGHT_MAX))

// Speed mapping (frame delay). Dark -> slower, Bright -> faster. Best guess pick..
#define DELAY_SLOW_MS    100
#define DELAY_FAST_MS    20

// similar macro to scale/translate raw ambient input to sweep speed output.
#define MAP_DELAY(raw) \
  ((int) map((long)(raw), AMBIENT_MIN_RAW, AMBIENT_MAX_RAW, DELAY_SLOW_MS, DELAY_FAST_MS))

// Helpers
#define ARRAY_LEN(a)     (sizeof(a) / sizeof((a)[0]))
#define DIST(a,b)        ((a) >= (b) ? ((a) - (b)) : ((b) - (a)))  // absolute distance

// PWM-capable pins on out board per lecture slides.
const uint8_t PINS[] = {3, 5, 6, 9, 10, 11};
const int LED_COUNT = ARRAY_LEN(PINS);

int pos = 0;   // current LED index (0..LED_COUNT-1)
int dir = 1;   // +1 forward, -1 back

void setup() {
  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(PINS[i], OUTPUT);
    analogWrite(PINS[i], 0);
  }
}

void loop() {
  // Read ambient light reading (raw =  0..1023) 
  int ambient_brightness = analogRead(LDR_PIN);

  uint8_t mapped_brightness = MAP_BRIGHTNESS(ambient_brightness);  // 0..255
  int frame_delay_ms = MAP_DELAY(ambient_brightness);               // DELAY_SLOW_MS..DELAY_FAST_MS

  // Draw one frame every loop
  for (int i = 0; i < LED_COUNT; i++) {
    int distance_from_head = DIST(i, pos);

    // Basic LED trail shape
    uint8_t base_pwm = (distance_from_head == 0) ? HEAD_PWM
                       : (distance_from_head == 1) ? FOLLOW1_PWM
                       : (distance_from_head == 2) ? FOLLOW2_PWM
                       : 0;

    // Now scale base by ambient brightness based on mapped brighness
    uint8_t pwm_out = (base_pwm * mapped_brightness) / 255;


    analogWrite(PINS[i], pwm_out);
  }

  // 3) Pace & move
  delay(frame_delay_ms);

  pos += dir;
  if (pos == 0 || pos == LED_COUNT - 1) dir = -dir;
}
