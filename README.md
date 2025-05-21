# 🚗 Smart Parking System using Arduino

This project is a **Smart Parking System** that automates gate control and slot detection using:

- IR sensors for vehicle detection
- Servo motors for gate movement
- An ultrasonic sensor to check parking slot availability
- An I2C LCD to display system status

It simulates a real-world single-slot parking system with entry and exit automation.

---

## 🧰 Components Used

| Component           | Purpose                               |
|--------------------|----------------------------------------|
| Arduino Uno/Nano   | Microcontroller                        |
| Servo Motor (x2)   | Entry and Exit Gate movement           |
| IR Sensor (x2)     | Vehicle detection at entry/exit        |
| Ultrasonic Sensor  | Parking slot availability check        |
| I2C LCD (16x2)     | Display system status                  |
| L293D Motor Shield | Power and pin access (optional)        |
| Jumper wires       | Circuit connections                    |

---

## 📌 Pin Configuration

| Sensor/Device       | Arduino Pin |
|---------------------|-------------|
| Entry IR Sensor     | A0          |
| Exit IR Sensor      | A1          |
| Entry Servo Motor   | D9          |
| Exit Servo Motor    | D10         |
| Ultrasonic TRIG     | A2          |
| Ultrasonic ECHO     | A3          |
| I2C LCD             | A4 (SDA), A5 (SCL) |

> You can adjust pin numbers in the code as needed.

---

## 🔧 How It Works

- **Entry Gate:**
  - Detects a car using IR sensor.
  - If the parking slot is **available**, it opens the gate (servo rotates), allowing the car in.
  - If the slot is **occupied**, it displays **"No Slots!"** and keeps the gate closed.

- **Exit Gate:**
  - Detects a car using IR sensor.
  - Opens the gate for exit and closes after the car passes.

- **Parking Slot Detection:**
  - Ultrasonic sensor continuously monitors distance.
  - If distance > 10 cm → slot is available.
  - If distance ≤ 10 cm → slot is occupied.

- **LCD Display:**
  - Displays real-time system states like:
    - Car detected
    - Opening/Closing gates
    - Slot availability

---

## 🔌 Setup Instructions

1. **Wiring:**
   - Connect components as per the pin configuration.
   - Ensure a stable power supply for servos.

2. **Install Libraries:**
   In the Arduino IDE:
   - Go to **Sketch > Include Library > Manage Libraries**
   - Install:
     - `Servo`
     - `LiquidCrystal_I2C`

3. **Upload the Code:**
   - Open the `.ino` file in Arduino IDE
   - Select the correct board and COM port
   - Upload the code

---

## 📟 Sample LCD Output

| LCD Line 1     | LCD Line 2         |
|----------------|--------------------|
| System Ready   |                    |
| Car Detected...| Opening Entry...   |
| Slot:Available |                    |
| Slot:Occupied  |                    |

---

## 🧪 Serial Monitor Output (115200 baud)

```plaintext
Servos initialized. Gates closed.
Entry and Exit IR sensors initialized.
Ultrasonic sensor initialized for 1 slot.
Car detected at the entrance. Checking parking availability...
Parking slot available. Opening entry gate...
Entry gate opened.
Closing Entry...
Entry gate closed.
