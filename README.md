# Foundation Pack 01 — Non-Blocking Super-Loop

## 1. Project Objective

Design, implement, and test a non-blocking embedded application using the PIC16F1827 microcontroller.

The application will control a LED heartbeat and respond to a push button that switches the system between two operating states: RUNNING and PAUSED.

The objective is to develop a solid understanding of GPIO configuration, state machines, timer-based scheduling, and non-blocking firmware architecture.

### Functional Requirements

- At power-up, the system enters the RUNNING state.
- While RUNNING, the LED toggles every 500 milliseconds.
- Pressing the button changes the system to PAUSED.
- While PAUSED, the LED remains OFF.
- Pressing the button again returns the system to RUNNING.
- The application must continuously monitor the button while managing the LED.
- The application must not use blocking delays such as `__delay_ms()`.

## 2. Hardware

### Development Equipment

| Component               | Description                                  |
| ----------------------- | -------------------------------------------- |
| Development board       | Microchip Demo Board, part number 02-01630-1 |
| Microcontroller         | PIC16F1827                                   |
| Programmer/debugger     | MPLAB ICD 3                                  |
| Development environment | MPLAB X IDE                                  |
| Compiler                | XC8                                          |
| Power supply            | Development board power supply               |

### Hardware Connections

| Component                     | Connection       | Description                                                          |
| ----------------------------- | ---------------  | -------------------------------------------------------------------- |
| D1                            | Power indicator  | Indicates board power                                                |
| D2                            | RB0 through J6   | LED selected for the heartbeat                                       |
| S2                            | RA4              | Push button selected for state transitions                           |
| Pull-up resistor              | 4.7 kΩ          | Already installed in the button circuit                              |
| LED current-limiting resistor | 470 Ω (Onboard) | Already installed in the Led circuit |

### Button Logic

The S2 push button is configured as an active-low input.

- Button released: logic HIGH (1).
- Button pressed: logic LOW (0).

The existing 4.7 kΩ pull-up resistor holds the input HIGH when the button is not pressed.

### Hardware Preparation

Before testing the firmware, verify that jumper J6 is configured to connect the onboard PORTB LEDs.

No additional external LED or push-button circuit is required for this project.

## 3. Super-Loop Architecture

**Status: Pending design and implementation.**

The application will use a non-blocking super-loop.

The firmware must continuously monitor the push button, manage application states, and update the LED without blocking execution.

### Engineering Work

- Design the main application loop.
- Determine the required initialization procedures.
- Define the application functions and their responsibilities.
- Implement non-blocking execution.

The final architecture and design decisions will be documented after implementation.

## 4. Application States

The application will have two operating states.

### RUNNING

- The LED toggles every 500 milliseconds.
- The push button remains responsive.
- A valid button press transitions the system to PAUSED.

### PAUSED

- The LED remains OFF.
- The push button remains responsive.
- A valid button press transitions the system to RUNNING.

### Engineering Work

Design and implement the state machine.

The application must distinguish between a new button press and a button that remains held down.

Button debouncing must be implemented to prevent unintended state transitions.

The selected state-machine design and transition logic will be documented during implementation.

## 5. Timing

**Status: Pending design and implementation.**

### Requirements

- LED toggle interval: 500 ms.
- Timing must be non-blocking.
- Button monitoring must continue while LED timing is in progress.
- Timer overflow must be handled correctly.

Timer1 will be used as a free-running timer, polled by the main application.

Timer interrupts are outside the scope of Foundation Pack 01.

### Engineering Work

Determine and document:

- Microcontroller clock configuration.
- Timer1 clock source and prescaler.
- Timer tick resolution.
- Method for calculating elapsed time.
- Method for handling timer overflow.

## 6. Test Evidence

**Status: Not started.**

| Test                   | Expected Result                             | Actual Result |
| ---------------------- | ------------------------------------------- | ------------- |
| Power-up               | System enters RUNNING                       | Pending       |
| LED heartbeat          | LED toggles every 500 ms                    | Pending       |
| Button press           | RUNNING changes to PAUSED                   | Pending       |
| PAUSED behavior        | LED remains OFF                             | Pending       |
| Resume                 | Button press returns to RUNNING             | Pending       |
| Button debounce        | One press produces one state transition     | Pending       |
| Non-blocking operation | Button remains responsive during LED timing | Pending       |

Actual results and supporting evidence will be recorded after firmware implementation and hardware testing.

## 7. What Failed and Troubleshooting

**Status: Not started.**

Document problems encountered during implementation and testing.

For each problem, record:

- Expected behavior.
- Actual behavior.
- Investigation and troubleshooting steps.
- Root cause.
- Corrective action and verification.

## 8. Engineering Takeaways

**Status: To be completed after testing.**

Document three engineering lessons learned from completing the project.

1. TBD
2. TBD
3. TBD

These takeaways must reflect actual engineering work, design decisions, debugging, and test results.
