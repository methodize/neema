/*
 * Neema - Potentiometer Controlled Stepper Motor
 *
 * This sketch reads a potentiometer value and controls a stepper motor's position.
 * The potentiometer position is mapped to the stepper motor's angular position.
 *
 * Hardware Requirements:
 * - Arduino board (Uno, Nano, Mega, etc.)
 * - NEMA stepper motor (NEMA 17 or similar)
 * - Stepper motor driver (A4988, DRV8825, or similar)
 * - Potentiometer (10K recommended)
 *
 * Wiring:
 * - Potentiometer middle pin to A0
 * - Potentiometer outer pins to 5V and GND
 * - Stepper driver STEP pin to digital pin 3
 * - Stepper driver DIR pin to digital pin 2
 * - Stepper driver ENABLE pin to digital pin 4 (optional)
 * - Connect stepper motor to driver according to driver documentation
 */

#include <AccelStepper.h>

// Pin definitions
const int POT_PIN = A0;           // Potentiometer analog input pin
const int STEP_PIN = 3;           // Step pulse pin
const int DIR_PIN = 2;            // Direction pin
const int ENABLE_PIN = 4;         // Enable pin (optional, set to -1 if not used)

// Stepper motor configuration
const int STEPS_PER_REVOLUTION = 200;  // Typical for NEMA 17 (1.8° per step)
const int MICROSTEPS = 16;             // Set according to your driver configuration
const int TOTAL_STEPS = STEPS_PER_REVOLUTION * MICROSTEPS;

// Motion parameters
const int MAX_SPEED = 1000;        // Maximum steps per second
const int ACCELERATION = 500;      // Steps per second^2

// Potentiometer reading configuration
const int POT_DEADBAND = 5;        // Analog read tolerance to reduce jitter
const int POT_MIN = 0;             // Minimum pot value
const int POT_MAX = 1023;          // Maximum pot value

// Initialize stepper with DRIVER interface (step, direction pins)
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Variables
int lastPotValue = -1;
int currentPotValue = 0;
long targetPosition = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Neema - Potentiometer Stepper Controller");
  Serial.println("=========================================");

  // Configure enable pin if used
  if (ENABLE_PIN >= 0) {
    pinMode(ENABLE_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, LOW);  // Enable driver (active LOW for most drivers)
  }

  // Configure stepper motor
  stepper.setMaxSpeed(MAX_SPEED);
  stepper.setAcceleration(ACCELERATION);
  stepper.setCurrentPosition(0);  // Set current position as zero point

  Serial.println("Setup complete!");
  Serial.print("Steps per revolution: ");
  Serial.println(TOTAL_STEPS);
  Serial.print("Max speed: ");
  Serial.println(MAX_SPEED);
  Serial.print("Acceleration: ");
  Serial.println(ACCELERATION);
  Serial.println();
}

void loop() {
  // Read potentiometer value
  currentPotValue = analogRead(POT_PIN);

  // Only update if the value has changed beyond the deadband
  if (abs(currentPotValue - lastPotValue) > POT_DEADBAND) {
    lastPotValue = currentPotValue;

    // Map potentiometer value to stepper position
    // Map full pot range to full rotation (0 to TOTAL_STEPS)
    targetPosition = map(currentPotValue, POT_MIN, POT_MAX, 0, TOTAL_STEPS);

    // Set new target position
    stepper.moveTo(targetPosition);

    // Print debug information
    Serial.print("Pot: ");
    Serial.print(currentPotValue);
    Serial.print(" | Target: ");
    Serial.print(targetPosition);
    Serial.print(" | Current: ");
    Serial.println(stepper.currentPosition());
  }

  // Run the stepper motor (this must be called frequently)
  stepper.run();
}
