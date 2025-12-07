/*
 * Neema - Potentiometer Controlled Stepper Motor for Blinds Tilt Control
 *
 * This sketch reads a potentiometer value and controls a stepper motor's position
 * to open/close window blind slats via the tilt wand mechanism.
 * The potentiometer position is mapped to the stepper motor's angular position.
 *
 * TESTED HARDWARE CONFIGURATION:
 * - Arduino Nano Every (ATmega4809)
 * - Stepperonline 17HS19-2004S1 NEMA 17 stepper (2.0A, 59Ncm torque)
 * - Pololu DRV8825 stepper driver
 * - 10K potentiometer
 * - 12V battery power (with optional buck converter for Arduino)
 *
 * IMPORTANT - DRV8825 Current Limiting:
 * Must set current limit to ~1.4A (70% of motor's 2.0A rating)
 * Formula: Vref = Current Limit / 2
 * Target Vref = 1.4A / 2 = 0.7V
 * Adjust trimpot on DRV8825 while measuring Vref pin voltage
 *
 * Wiring:
 * - Potentiometer middle pin to A0
 * - Potentiometer outer pins to 5V and GND
 * - DRV8825 STEP pin to digital pin 3
 * - DRV8825 DIR pin to digital pin 2
 * - DRV8825 ENABLE pin to digital pin 4
 * - DRV8825 M0, M1, M2 to configure microstepping (all HIGH = 1/32)
 * - DRV8825 VMOT to 12V battery positive
 * - DRV8825 GND to battery negative
 * - Connect motor coils to DRV8825 A1, A2, B1, B2
 * - Arduino VIN to 12V (or use buck converter to 5V pin)
 */

#include <AccelStepper.h>

// Pin definitions
const int POT_PIN = A0;           // Potentiometer analog input pin
const int STEP_PIN = 3;           // Step pulse pin
const int DIR_PIN = 2;            // Direction pin
const int ENABLE_PIN = 4;         // Enable pin (optional, set to -1 if not used)

// Stepper motor configuration
const int STEPS_PER_REVOLUTION = 200;  // 17HS19-2004S1: 1.8° per step = 200 steps/rev
const int MICROSTEPS = 32;             // DRV8825 with M0,M1,M2 HIGH = 1/32 microstepping
const int TOTAL_STEPS = STEPS_PER_REVOLUTION * MICROSTEPS;  // 6400 steps per revolution

// Motion parameters
const int MAX_SPEED = 2000;        // Maximum steps per second (adjust for smoothness)
const int ACCELERATION = 1000;     // Steps per second^2 (adjust for responsiveness)

// Blinds-specific configuration
// Adjust ROTATION_RANGE to limit travel (180 = half rotation for typical blinds)
// Set to TOTAL_STEPS for full 360° rotation if needed
const int ROTATION_RANGE = TOTAL_STEPS / 2;  // 180° range for blinds tilt

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
  Serial.print("Rotation range: ");
  Serial.print(ROTATION_RANGE);
  Serial.print(" steps (");
  Serial.print((ROTATION_RANGE * 360) / TOTAL_STEPS);
  Serial.println(" degrees)");
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
    // Map full pot range to configured rotation range (default 180° for blinds)
    targetPosition = map(currentPotValue, POT_MIN, POT_MAX, 0, ROTATION_RANGE);

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
