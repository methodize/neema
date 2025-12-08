# Neema - Window Blinds Tilt Control

An Arduino project that controls window blind slats using a potentiometer and stepper motor. Turn the potentiometer to open or close the blinds by rotating the tilt wand mechanism. Perfect for automated or remote-controlled window blinds.

## Features

- Smooth blinds slat control with acceleration/deceleration
- Potentiometer input for manual position control
- Configurable rotation range (default 180° for typical blinds)
- Real-time position feedback via Serial monitor
- Deadband filtering to reduce jitter from potentiometer noise
- 1/32 microstepping for precise, quiet operation
- Battery-powered operation (12V)
- Easy configuration for different blind types

## Hardware Requirements

### Tested Configuration
This project has been tested and configured for:
- **Arduino Nano Every** (ATmega4809) - Board definition: `arduino:megaavr:nona4809`
- **Stepperonline 17HS19-2004S1** - NEMA 17 stepper motor
  - Rated current: 2.0A per phase
  - Holding torque: 59Ncm (83.55 oz-in)
  - Step angle: 1.8° (200 steps/revolution)
  - Resistance: 1.5Ω per phase
- **Pololu DRV8825 Stepper Driver**
  - Supports up to 2.2A per coil (sufficient for 2.0A motor)
  - 1/32 microstepping capability
  - Built-in current limiting
- **10K Potentiometer** (3-terminal linear taper)
- **12V Battery** for power
- **Buck converter** (optional, for powering Arduino from 12V)
- Jumper wires and breadboard for prototyping

### Wiring Diagram

#### Potentiometer
- Middle pin → Arduino A0
- One outer pin → Arduino 5V
- Other outer pin → Arduino GND

#### DRV8825 Stepper Driver Connections
**Control Pins:**
- STEP → Arduino pin 3 (D3)
- DIR → Arduino pin 2 (D2)
- ENABLE → Arduino pin 4 (D4)
- SLEEP → Connect to RESET pin (or Arduino 5V to keep enabled)
- RESET → Connect to SLEEP pin (or Arduino 5V)

**Microstepping Configuration (for 1/32 mode):**
- M0 → Arduino 5V (or tie HIGH)
- M1 → Arduino 5V (or tie HIGH)
- M2 → Arduino 5V (or tie HIGH)

**Power:**
- VMOT → 12V battery positive
- GND → 12V battery negative AND Arduino GND (common ground)
- VDD → Arduino 5V (logic power)

**Motor Connections:**
- A1, A2 → Motor coil A (one pair from motor)
- B1, B2 → Motor coil B (other pair from motor)

#### Arduino Nano Every Power Options

**Option 1: Direct 12V to VIN** (Recommended for battery operation)
- 12V battery positive → Arduino VIN pin
- 12V battery negative → Arduino GND

**Option 2: Buck Converter to 5V**
- 12V battery → Buck converter input
- Buck converter output (set to 5V) → Arduino 5V pin
- Common GND between all components

**Important:** The DRV8825 can get HOT. Consider adding a heatsink if running continuously at high current.

**CRITICAL - Common Ground:** All components MUST share a common ground. If using multiple power supplies (e.g., separate 5V for Arduino and 12V for motor), ensure all grounds are connected together. Without common ground, the motor may energize but not step.

### DRV8825 Current Limiting Setup (CRITICAL!)

**You MUST set the current limit on the DRV8825 before powering the motor.** Failure to do so can damage the motor or driver.

For the 17HS19-2004S1 (2.0A motor), set current limit to approximately **1.4A** (70% of rated current):

1. **Tools needed:**
   - Small flathead screwdriver (for trimpot adjustment)
   - Multimeter

2. **Procedure:**
   - Power the DRV8825 (12V to VMOT, GND connected, Arduino powered)
   - Set multimeter to DC voltage mode
   - Measure voltage between the trimpot wiper and GND
   - Adjust trimpot to get **0.7V** (Vref = Current / 2 = 1.4A / 2)
   - Start low and gradually increase while testing motor

3. **Formula:**
   ```
   Current Limit = Vref × 2
   Target: 1.4A → Vref = 0.7V
   ```

4. **Testing:**
   - Start with Vref around 0.5V (1.0A) for initial testing
   - If motor skips steps or lacks torque, increase slightly
   - If motor gets too hot, decrease current
   - Don't exceed 0.8V (1.6A) for this motor

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
3. Wire up all components according to wiring diagram
4. **Set DRV8825 current limit** (see section above - DO THIS FIRST!)
5. Connect Arduino Nano Every via USB
6. Upload the sketch:

**Using Arduino IDE:**
- Select Board: "Arduino Nano Every"
- Select Port: Your USB port
- Click Upload

**Using Arduino CLI:**
```bash
# Install required board support
arduino-cli core install arduino:megaavr

# Compile and upload
arduino-cli compile --fqbn arduino:megaavr:nona4809 neema.ino
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:megaavr:nona4809 neema.ino

# Monitor serial output
arduino-cli monitor -p /dev/ttyUSB0 -c baudrate=115200
```

## Configuration

You can adjust the following parameters in the sketch to customize for your specific blinds:

### Pin Configuration
```cpp
const int POT_PIN = A0;        // Potentiometer input
const int STEP_PIN = 3;        // Step pulse pin
const int DIR_PIN = 2;         // Direction pin
const int ENABLE_PIN = 4;      // Enable pin
```

### Motor Configuration
```cpp
const int STEPS_PER_REVOLUTION = 200;  // 17HS19-2004S1: 1.8° per step
const int MICROSTEPS = 32;             // DRV8825: 1/32 microstepping (M0,M1,M2 HIGH)
const int TOTAL_STEPS = 6400;          // 200 × 32 = 6400 steps per revolution
```

### Blinds-Specific Settings
```cpp
const int ROTATION_RANGE = TOTAL_STEPS / 2;  // 180° (half rotation)
// Adjust this value based on your blinds:
// - TOTAL_STEPS / 4 = 90° rotation
// - TOTAL_STEPS / 2 = 180° rotation  (typical for most blinds)
// - TOTAL_STEPS = 360° full rotation
```

### Motion Parameters
```cpp
const int MAX_SPEED = 2000;        // Maximum steps per second (higher for 1/32 microstepping)
const int ACCELERATION = 1000;     // Steps per second² (smooth but responsive)
```

### Potentiometer Settings
```cpp
const int POT_DEADBAND = 5;        // Reduce to 2-3 for more sensitivity
                                   // Increase to 10-15 to reduce jitter
```

## Usage

1. Ensure DRV8825 current limit is set correctly
2. Power the system with 12V battery
3. Upload the sketch to Arduino Nano Every
4. Open Serial Monitor (115200 baud) for debugging
5. Turn the potentiometer - the motor rotates the tilt wand
6. Blinds slats should open/close following the potentiometer position

### First-Time Setup Tips

1. **Test without blinds attached first**
   - Verify motor rotates smoothly
   - Check rotation direction (swap motor coil pairs if needed)
   - Ensure no skipping or stuttering

2. **Attach to blinds tilt wand**
   - You may need a coupling or adapter to connect motor shaft to wand
   - Common options: flexible shaft coupling, 3D printed adapter, or direct friction fit

3. **Calibrate rotation range**
   - Note pot positions for fully open and fully closed
   - Adjust `ROTATION_RANGE` if needed (typically 90-180°)

### Serial Output Example
```
Neema - Potentiometer Stepper Controller
=========================================
Setup complete!
Steps per revolution: 6400
Rotation range: 3200 steps (180 degrees)
Max speed: 2000
Acceleration: 1000

Pot: 512 | Target: 1600 | Current: 1598
Pot: 768 | Target: 2400 | Current: 2398
Pot: 256 | Target: 800 | Current: 802
```

## Troubleshooting

### Motor doesn't move at all
- **Check DRV8825 current limit** - Must be set before motor will work properly
- Verify all wiring connections (especially SLEEP and RESET tied together or to 5V)
- Ensure 12V battery is charged and providing adequate voltage
- Check that ENABLE pin is LOW (sketch sets this automatically)
- Verify M0, M1, M2 pins are HIGH for 1/32 microstepping
- Measure VDD on DRV8825 should be 5V

### Motor whirs/vibrates but doesn't turn
- **MOST COMMON:** Missing common ground between power supplies
  - If using separate 5V and 12V supplies, ALL grounds must be connected together
  - Arduino GND, DRV8825 GND, and all power supply grounds must be common
  - Without common ground, control signals don't work properly
- **Current too low** - Increase Vref on DRV8825 (try 0.6-0.7V)
- **Speed too high** - Motor can't keep up, reduce MAX_SPEED to 400 for testing
- Motor shaft has mechanical resistance - check coupling/mounting

### Motor jitters or stutters
- **Current too low** - Increase Vref on DRV8825 slightly (don't exceed 0.8V)
- **Current too high** - Motor will get hot; decrease Vref
- Increase `POT_DEADBAND` to reduce sensitivity (try 10-15)
- Lower `MAX_SPEED` if motor is skipping steps (try 1000-1500)
- Ensure stable 12V power supply (weak battery will cause issues)
- Add 100µF capacitor across DRV8825 VMOT and GND

### Erratic movement or jumpy response
- Check potentiometer wiring (especially middle pin to A0)
- Verify potentiometer is good quality (cheap pots can be noisy)
- Increase `POT_DEADBAND` value to 10 or higher
- Add 0.1µF capacitor between A0 and GND for noise filtering
- Ensure common ground between Arduino and DRV8825

### Motor gets too hot
- **Current limit too high** - Reduce Vref on DRV8825 (target 0.7V max)
- Add heatsink to DRV8825 chip
- Ensure adequate airflow
- Consider reducing holding current when motor is idle

### Wrong rotation direction
- Swap one motor coil pair (either A1/A2 or B1/B2, not both)
- Or modify code to invert direction

### Blinds-Specific Issues

**Motor doesn't have enough torque:**
- Increase current limit slightly (up to 0.8V Vref)
- Reduce `MAX_SPEED` for more torque
- Check tilt wand mechanism isn't binding
- Verify motor is properly coupled to wand

**Blinds don't fully open/close:**
- Adjust `ROTATION_RANGE` to match your blinds' actual travel
- Typical blinds use 90-180° of rotation
- Monitor serial output to see actual position values

**Battery drains too quickly:**
- Motor holds position using current even when stopped
- Consider adding auto-sleep after period of inactivity
- Use lower current limit if holding torque not critical

## Future Enhancements

Potential improvements for this blinds controller:

- [ ] **Auto-sleep mode** - Disable motor after inactivity to save battery
- [ ] **Position presets** - Buttons for "fully open", "fully closed", "50%" positions
- [ ] **EEPROM position storage** - Remember last position on power loss
- [ ] **WiFi/Bluetooth control** - ESP32 integration for smartphone app control
- [ ] **Scheduled operation** - RTC module for automatic open/close times
- [ ] **Light sensor integration** - Auto-close blinds when too bright
- [ ] **Multiple blinds control** - Single controller for several windows
- [ ] **Manual override detection** - Detect if blinds moved manually and resync
- [ ] **Battery voltage monitoring** - Alert when battery needs charging
- [ ] **Current sensing** - Detect if motor stalls (blind jammed)

## License

This project is open source. Feel free to modify and use for your own projects.

## Author

Created for the Shed Computer Arduino projects collection.

## Bill of Materials

| Component | Part Number/Model | Approx. Cost |
|-----------|------------------|--------------|
| Arduino Nano Every | ATmega4809 | $10-15 |
| Stepper Motor | Stepperonline 17HS19-2004S1 | $10-15 |
| Stepper Driver | Pololu DRV8825 | $8-12 |
| Potentiometer | 10K linear taper | $1-3 |
| 12V Battery | Li-ion or SLA | $15-30 |
| Buck Converter (optional) | LM2596 module | $2-5 |
| Misc. | Wires, breadboard, heatsink | $5-10 |

**Total estimated cost:** $50-90 USD

## References

- [AccelStepper Library Documentation](http://www.airspayce.com/mikem/arduino/AccelStepper/)
- [Stepperonline 17HS19-2004S1 Product Page](https://www.omc-stepperonline.com/nema-17-bipolar-59ncm-84oz-in-2a-42x48mm-4-wires-w-1m-cable-connector-17hs19-2004s1)
- [Pololu DRV8825 Stepper Driver](https://www.pololu.com/product/2133)
- [DRV8825 Current Limiting Guide](https://www.pololu.com/product/2133#current-limiting-guide)
- [Arduino Nano Every Documentation](https://docs.arduino.cc/hardware/nano-every)
