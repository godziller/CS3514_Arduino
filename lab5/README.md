I can certainly convert the README summary into a detailed Markdown description, suitable for a project documentation page.

---

# ⚡️ Reaction Time Tester (Direct Port Manipulation)

This project implements a digital reaction time tester game using an **Arduino** (or compatible ATmega328p microcontroller), focusing on **direct port manipulation** to achieve microsecond-level timing precision and efficiency by minimizing software overhead.

---

## 🎯 Game Objective

The player's goal is to press the **Stop Button** as quickly as possible after the **LED** (the "GO" signal) illuminates, measuring their reaction time with high accuracy.

---

## 🔌 Hardware Configuration

All primary inputs and outputs for this game are consolidated onto **PORTD** for simplified register control. The code utilizes the **internal pull-up resistors** for the buttons.

| Component | Arduino Digital Pin | Port Bit | Function | Wiring (Required) |
| :--- | :--- | :--- | :--- | :--- |
| **Start Button** | **Pin 4** | `PD4` | Initiates the random wait sequence. | Pin $\leftrightarrow$ **GND** |
| **Stop Button** | **Pin 5** | `PD5` | Records the reaction time (Stop signal). | Pin $\leftrightarrow$ **GND** |
| **Start LED** | **Pin 6** | `PD6` | The visual "GO" signal (Output). | Pin $\leftrightarrow$ **Resistor $\leftrightarrow$ GND** |

---

## ⚙️ Direct Register Control

The project uses custom **macros** to directly access and control the microcontroller's low-level registers (`DDR`, `PORT`, `PIN`), bypassing the slower Arduino function calls.

### **Macro Definitions**

| Macro Family | Target Register | Purpose |
| :--- | :--- | :--- |
| `SET_BIT_D` / `CLR_BIT_D` | **PORTD** | Sets the pin's voltage (HIGH/LOW) for **Outputs**, or enables/disables the **Pull-up** for Inputs. |
| `SET_DDRD_OUT` / `SET_DDRD_IN` | **DDRD** | Configures the pin direction as **Output ($1$)** or **Input ($0$)**. |
| `READ_BIT_D` | **PIND** | Reads the current logical state of the physical pin. |

### **Configuration Summary**

* **LED (Pin 6):** Configured as an **Output** using `SET_DDRD_OUT(PD6)`.
* **Buttons (Pins 4 & 5):** Configured as **Inputs** using `SET_DDRD_IN()` and have **internal pull-ups enabled** using `SET_BIT_D()` on their respective bits.

---

## 💡 Game Flow and Logic

The game logic is structured to prevent false starts and provide precise timing:

1.  **Start Sequence:** The system waits in a `while (READ_BIT_D(START_BUTTON_BIT))` loop until **Pin 4** is pressed (goes **LOW**) and then waits for it to be released.
2.  **Random Delay:** A random wait time (`WAIT_MIN` ($0\text{ ms}$) to `WAIT_MAX` ($10,000\text{ ms}$)) is generated.
3.  **False Start Check:** During the random delay, the code continuously checks the **Stop Button (Pin 5)**. If pressed prematurely, the player is penalized, and all running scores are reset via `resetScoreboard()`.
4.  **GO Signal & Timing:** When the delay ends, the **LED (Pin 6)** is turned **ON**, and the reaction timer is started using `startTime = micros()`.
5.  **Reaction Capture:** The code waits for the **Stop Button (Pin 5)** to be pressed. The final reaction time is calculated to microsecond precision, and results (current time, best time, average time) are reported via the Serial Monitor.
