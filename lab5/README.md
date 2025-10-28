# ⚡️ High-Speed Arduino Reaction Timer

This Arduino sketch implements a highly accurate, score-tracking reaction time tester featuring **Direct Port Manipulation** for minimal latency and a **harsh penalty** system for false starts.

---

## 🚀 Features

* **Microsecond Accuracy:** Uses the `micros()` function for precise reaction timing.
* **Direct Port Control:** Leverages **PORTB macros** (`SET_BIT_B`, `CLR_BIT_B`, `READ_BIT_B`) to read and write pins directly, avoiding the overhead of `digitalRead()` and `digitalWrite()`.
* **Randomized Wait:** Implements a random delay between **0 and 10 seconds** before the GO signal to prevent anticipation.
* **Score Tracking:** Calculates and reports the **Best Time** and a **Running Average** across multiple games.
* **Forfeit Penalty:** If the player presses the button too early (False Start), they **forfeit the game**, and the entire **scoreboard is reset** to zero.

---

## 🔌 Hardware Setup

This code is written for an **Arduino Uno** (or similar ATmega328P-based board) and uses specific pins mapped to **PORTB**.

| Component | Arduino Pin (Digital) | PORTB Bit | Function |
| :--- | :--- | :--- | :--- |
| **Start LED** | D8 | PB0 (`START_LED_BIT`) | Lights up to signal 'GO!' |
| **Reaction Button** | D13 | PB5 (`BUTTON_BIT`) | Used to start and stop the timer |

**Wiring Notes:**
1.  The **Reaction Button** is connected between **Pin D13** and **GND**. The internal pull-up resistor is enabled in `setup()` to keep the pin HIGH when unpressed.
2.  The **Start LED** is connected to **Pin D8** (with a current-limiting resistor) and **GND**.

---

## ⚙️ Core Logic Breakdown

The game runs through the following phases, handled within the main `loop()` function:

### 1. Wait for Start
The code uses two sequential `while` loops to manage the initial button press:
* Waits for the button press (LOW).
* Waits for the **button release** (HIGH) to ensure the player's finger is lifted before the random wait begins.

### 2. Random Wait & False Start Check (The Crucial Part)
A random time (0-10000ms) is generated. The code loops, waiting for this time to elapse.
* **Forfeit Condition:** Inside this loop, an `if` statement constantly checks the button state. If the button is pressed (`!READ_BIT_B(BUTTON_BIT)`), the **false start** is triggered:
    * The **`resetScoreboard()`** function is called, wiping all previous best and average data.
    * A harsh "TOO IMPATIENT" message is printed.
    * `return;` is used to immediately exit the current game loop and start the next game cycle.

### 3. GO Signal & Timing
* The LED on D8 is lit (`SET_BIT_B(START_LED_BIT)`).
* The high-resolution timer starts immediately (`startTime = micros();`).
* The code enters a final tight `while` loop waiting for the reaction press.

### 4. Results and Reporting
* `reactionTime` is calculated.
* The **`reportResults()`** function is called to handle all post-game tasks:
    * Increments `gameCount`.
    * Updates `totalReactionTime` and recalculates `averageReactionTime`.
    * Compares current time to `bestTime` and updates if faster.
    * Prints the current result and the running scoreboard to the Serial Monitor.

---

## 🛠️ Key Functions

| Function | Purpose |
| :--- | :--- |
| `setup()` | Initializes the Serial Monitor, seeds the random number generator using `micros()`, and sets up Pin D8 (Output) and Pin D13 (Input with Pull-up). |
| `reportResults()` | **Calculates and displays all scoring statistics** (best time, average time, total games played) after a successful game. |
| `resetScoreboard()` | **Resets all scoring variables** to their initial state (`bestTime` to max, `totalReactionTime` and `gameCount` to zero) upon a false start. |
