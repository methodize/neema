# Neema - Potentiometer Controlled Stepper Motor

An Arduino project that controls a NEMA stepper motor position using a potentiometer. The motor moves to match the angular position of the potentiometer, providing smooth and precise position control.

## Features

- Smooth stepper motor control with acceleration/deceleration
- Real-time position feedback via Serial monitor
- Configurable speed and acceleration
- Deadband filtering to reduce jitter
- Support for microstepping configurations
- Easy pin configuration

## Hardware Requirements

### Components
- Arduino board (Uno, Nano, Mega, or compatible)
- NEMA stepper motor (NEMA 17 recommended)
- Stepper motor driver (A4988, DRV8825, or similar)
- 10K Potentiometer
- 12V power supply for stepper motor (voltage depends on your motor)
- Breadboard and jumper wires

### Wiring Diagram

#### Potentiometer
- Middle pin → Arduino A0
- One outer pin → Arduino 5V
- Other outer pin → Arduino GND

#### Stepper Driver
- STEP → Arduino pin 3
- DIR → Arduino pin 2
- ENABLE → Arduino pin 4 (optional)
- VDD → Arduino 5V
- GND → Arduino GND
- VMOT → 12V power supply positive
- GND → 12V power supply negative
- 1A, 1B, 2A, 2B → Stepper motor coils

**Important:** Ensure your stepper motor power supply can provide sufficient current for your motor. Check motor specifications.

## Software Requirements

### Libraries
This project requires the AccelStepper library:

1. Open Arduino IDE
2. Go to Sketch → Include Library → Manage Libraries
3. Search for "AccelStepper"
4. Install "AccelStepper" by Mike McCauley

Or install via Arduino CLI:
```bash
arduino-cli lib install AccelStepper
```

## Installation

1. Clone this repository or download the sketch
2. Install the AccelStepper library (see above)
3. Open `neema.ino` in Arduino IDE
4. Connect your Arduino via USB
5. Select your board and port from Tools menu
6. Upload the sketch

## Configuration

You can adjust the following parameters in the sketch:

### Pin Configuration
```cpp
const int POT_PIN = A0;        // Potentiometer input
const int STEP_PIN = 3;        // Step pulse pin
const int DIR_PIN = 2;         // Direction pin
const int ENABLE_PIN = 4;      // Enable pin (-1 to disable)
```

### Motor Configuration
```cpp
const int STEPS_PER_REVOLUTION = 200;  // Motor steps (typically 200 for 1.8°/step)
const int MICROSTEPS = 16;             // Driver microstepping (1, 2, 4, 8, 16, 32)
```

### Motion Parameters
```cpp
const int MAX_SPEED = 1000;        // Maximum steps per second
const int ACCELERATION = 500;      // Steps per second²
```

## Usage

1. Upload the sketch to your Arduino
2. Open Serial Monitor (115200 baud)
3. Turn the potentiometer
4. The stepper motor will move to match the potentiometer position
5. Serial monitor displays current potentiometer value and motor position

### Serial Output Example
```
Neema - Potentiometer Stepper Controller
=========================================
Setup complete!
Steps per revolution: 3200
Max speed: 1000
Acceleration: 500

Pot: 512 | Target: 1600 | Current: 1598
Pot: 768 | Target: 2400 | Current: 2398
Pot: 256 | Target: 800 | Current: 802
```

## Troubleshooting

### Motor doesn't move
- Check wiring connections
- Verify power supply is connected and sufficient
- Ensure ENABLE pin is LOW (if using enable)
- Check driver microstepping configuration matches code

### Motor jitters or vibrates
- Adjust `POT_DEADBAND` value (increase to reduce sensitivity)
- Lower `MAX_SPEED` and `ACCELERATION` values
- Check motor current adjustment on driver
- Ensure stable power supply

### Erratic movement
- Check potentiometer wiring
- Verify potentiometer is good quality (cheap pots can be noisy)
- Add filtering capacitor (0.1µF) across potentiometer signal and ground

## Future Enhancements

- [ ] Add LCD display for position feedback
- [ ] Implement position presets with buttons
- [ ] Add homing routine with limit switch
- [ ] Store position in EEPROM on power loss
- [ ] Add speed control via second potentiometer
- [ ] Implement multiple motor control

## License

This project is open source. Feel free to modify and use for your own projects.

## Author

Created for the Shed Computer Arduino projects collection.

## References

- [AccelStepper Library Documentation](http://www.airspayce.com/mikem/arduino/AccelStepper/)
- [A4988 Stepper Driver Datasheet](https://www.pololu.com/product/1182)
- [NEMA 17 Stepper Motor Specifications](https://www.omc-stepperonline.com/nema-17-stepper-motor)
