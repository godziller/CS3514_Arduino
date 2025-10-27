// J.P.M Nov 2020
// Toggle bit pattern on AVR PORTD (e.g., Arduino Uno’s digital pins D0–D7).

// The bit mask for the pins we want to toggle.
// 0x3C (hex)  = 0011 1100 (binary)
//               |||| ||||
//               7654 3210  (bit numbers; bit0 is LSB)
// This targets bits 2,3,4,5 (PD2–PD5). On an Arduino Uno:
//   PD2 -> D2, PD3 -> D3, PD4 -> D4, PD5 -> D5
#define BITS_TO_TOGGLE 0x3C  // 00111100

void setup() {
  // DDRD: Data Direction Register for Port D.
  //   1 = output, 0 = input
  // B11111111 (binary) sets all eight pins on PORTD (PD0–PD7) as outputs.
  // On Arduino Uno, that’s pins D0–D7.
  // WARNING: PD0/PD1 (D0/D1) are used for serial RX/TX; setting them as outputs
  // can interfere with Serial. If you need Serial, don’t set those as outputs.
  DDRD  = B11111111;

  // PORTD: Data Register for Port D.
  // Writing a 1 drives the output HIGH on pins configured as outputs.
  // Here we apply our mask so PD2–PD5 start HIGH, others LOW.
  // Result after this line:
  //   PD5..PD2 = 1 1 1 1  (HIGH)
  //   PD1..PD0 = 0 0      (LOW)
  //   PD7..PD6 = 0 0      (LOW)  (since BITS_TO_TOGGLE doesn’t set those bits)
  PORTD = BITS_TO_TOGGLE;
}

void loop() {
  // XOR (“toggle”) the masked bits on PORTD every iteration.
  //   x ^ 1 -> flips bit,  x ^ 0 -> leaves bit as-is.
  // Effect:
  //   PD5..PD2: flip each time (HIGH->LOW->HIGH->…)
  //   Other PD bits: unchanged
  PORTD ^= BITS_TO_TOGGLE;

  // Wait 500 ms so you can see the blinking on connected LEDs.
  delay(500);
}
