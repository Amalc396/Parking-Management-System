#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// I2C LCD address and dimensions
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Pin definitions for L293D motor driver shield
#define ENTRY_IR_SENSOR_PIN A0   // Analog pin A0 for Entry IR sensor
#define EXIT_IR_SENSOR_PIN A1   // Analog pin A1 for Exit IR sensor
#define ENTRY_SERVO_PIN 9       // Pin 9 for Entry Gate Servo
#define EXIT_SERVO_PIN 10       // Pin 10 for Exit Gate Servo
#define TRIG_PIN_SLOT_1 A2      // Analog pin A2 for Ultrasonic sensor TRIG
#define ECHO_PIN_SLOT_1 A3      // Analog pin A3 for Ultrasonic sensor ECHO

// Servo objects
Servo entryGateServo;
Servo exitGateServo;

// LCD object
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// Variables
bool isEntryGateOpen = false;      // Track entry gate state
bool isExitGateOpen = false;       // Track exit gate state
const int distanceThreshold = 10; // Threshold distance in cm for detecting a parked car

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);

  // Initialize the servos
  entryGateServo.attach(ENTRY_SERVO_PIN);
  exitGateServo.attach(EXIT_SERVO_PIN);
  entryGateServo.write(0); // Initial position (closed)
  exitGateServo.write(0);  // Initial position (closed)
  Serial.println("Servos initialized. Gates closed.");

  // Initialize IR sensor pins
  pinMode(ENTRY_IR_SENSOR_PIN, INPUT);
  pinMode(EXIT_IR_SENSOR_PIN, INPUT);
  Serial.println("Entry and Exit IR sensors initialized.");

  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN_SLOT_1, OUTPUT);
  pinMode(ECHO_PIN_SLOT_1, INPUT);
  Serial.println("Ultrasonic sensor initialized for 1 slot.");

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(2000);
}

void loop() {
  int entryIRValue = digitalRead(ENTRY_IR_SENSOR_PIN); // Read the Entry IR sensor state
  int exitIRValue = digitalRead(EXIT_IR_SENSOR_PIN);   // Read the Exit IR sensor state

  // Handle car entry
  if (entryIRValue == LOW && !isEntryGateOpen) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Car Detected...");
    Serial.println("Car detected at the entrance. Checking parking availability...");
    if (isParkingAvailable(TRIG_PIN_SLOT_1, ECHO_PIN_SLOT_1)) {
      lcd.setCursor(0, 1);
      lcd.print("Opening Entry...");
      Serial.println("Parking slot available. Opening entry gate...");
      openEntryGate();
      delay(3000); // Wait for the car to pass
    } else {
      lcd.setCursor(0, 1);
      lcd.print("No Slots!");
      Serial.println("No parking slots available. Entry gate remains closed.");
      delay(3000);
    }
  }

  // Handle car exit
  if (exitIRValue == LOW && !isExitGateOpen) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Car Exiting...");
    Serial.println("Car detected at the exit. Opening exit gate...");
    openExitGate();
    delay(3000); // Wait for the car to exit
  }

  // Close gates if no cars are detected
  if (entryIRValue == HIGH && isEntryGateOpen) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Closing Entry...");
    closeEntryGate();
    delay(2000);
  }

  if (exitIRValue == HIGH && isExitGateOpen) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Closing Exit...");
    closeExitGate();
    delay(2000);
  }

  // Update parking slot status on LCD
  updateLCDSlotStatus();
}

// Function to open the entry gate
void openEntryGate() {
  entryGateServo.write(90); // Rotate servo to 90 degrees (open)
  isEntryGateOpen = true;
  Serial.println("Entry gate opened.");
}

// Function to close the entry gate
void closeEntryGate() {
  entryGateServo.write(0); // Rotate servo back to 0 degrees (closed)
  isEntryGateOpen = false;
  Serial.println("Entry gate closed.");
}

// Function to open the exit gate
void openExitGate() {
  exitGateServo.write(90); // Rotate servo to 90 degrees (open)
  isExitGateOpen = true;
  Serial.println("Exit gate opened.");
}

// Function to close the exit gate
void closeExitGate() {
  exitGateServo.write(0); // Rotate servo back to 0 degrees (closed)
  isExitGateOpen = false;
  Serial.println("Exit gate closed.");
}

// Function to check if a specific parking slot is available
bool isParkingAvailable(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  return distance > distanceThreshold;
}

// Function to update parking slot status on the LCD
void updateLCDSlotStatus() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Slot:");
  lcd.print(isParkingAvailable(TRIG_PIN_SLOT_1, ECHO_PIN_SLOT_1) ? "Available" : "Occupied");
  delay(1000);
}
