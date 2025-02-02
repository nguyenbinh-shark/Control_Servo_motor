# ATmega16 Servo Control Project

## Project Overview
This project uses an ATmega16 microcontroller to control a 2-degree-of-freedom (2DOF) mechanism, simulating a camera mount. Instead of a camera, a DC motor is attached to the mount. The system provides the following functionalities:

- **Servo Control:** Adjust the pitch and yaw angles of the mount between -45° and 45° using two servo motors.
- **DC Motor Speed Control:** Regulate the speed of a DC motor attached to the mount through PWM signals.
- **LCD Display:** Show real-time updates of motor speed (as a percentage) and servo positions on a 16x2 LCD.

## Features
- **Precise Angular Movement:** Servo motors controlled via ADC inputs ensure smooth and accurate positioning.
- **Dynamic Speed Control:** The DC motor's speed is adjustable and monitored in real-time.
- **User-Friendly Interface:** The LCD provides clear and continuous feedback on system status.

## Components
- **Microcontroller:** ATmega16
- **Servo Motors:** 2x Standard servo motors
- **DC Motor:** Replacing the camera for demonstration purposes
- **LCD Display:** 16x2 LCD to display speed and angles
- **Power Supply:** Ensures stable operation of all components
- **Additional Components:** Resistors, capacitors, and a crystal oscillator for stability and clocking

## Circuit Overview
- Servo motors connected to PWM pins OC1A (PD5) and OC1B (PD4).
- DC motor controlled via PWM on pin OC2 (PD7).
- LCD connected to PORTC for data and control lines.
- A stable power supply supports all components, with decoupling capacitors for noise reduction.

## Usage
1. Program the ATmega16 with the provided firmware.
2. Connect all components according to the circuit diagram.
3. Power on the system.
4. Adjust input controls to observe changes in motor speed and servo positions, displayed on the LCD.

## Repository Structure
```
|-- src/
|   |-- main.c       // Contains the project implementation
|-- README.md        // Project documentation
```

## VIDEO
[Video demo]{https://youtu.be/pzzcMIFderI}
## Author
[Your Name]  
[Contact Information]
