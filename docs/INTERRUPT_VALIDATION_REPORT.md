# Foundation Pack 01 — Timer1 Interrupt Upgrade

## Hardware Validation and Engineering Closeout

**Date:** November 15, 2026  
**Target:** PIC16F1827  
**Branch:** `feature/timer1-interrupt`  
**Instrument:** Rigol DS1054Z  
**Assessment:** PASS

## 1. Objective

Migrate the existing polling-based Timer1 heartbeat implementation to an interrupt-driven architecture while preserving the application state machine, button debounce mechanism and RUNNING/PAUSED behavior.

## 2. Timer Configuration

| Parameter                 | Value         |
| ------------------------- | ------------- |
| Oscillator frequency      | 500 kHz       |
| Timer1 clock source       | FOSC/4        |
| Prescaler                 | 1:1           |
| Timer tick                | 8 µs          |
| Required counts           | 62,500        |
| Preload                   | 3036 (0x0BDC) |
| Nominal overflow interval | 500 ms        |

An alternative 1:8 prescaler configuration was independently calculated. Its fractional count requirement of 7812.5 introduces a quantization error of ±32 µs, equivalent to 0.0064%.

The 1:1 configuration was retained to avoid this nominal quantization error.

## 3. Firmware Architecture

Timer1 overflow processing is performed inside the interrupt service routine. The ISR checks the interrupt enable and flag bits, reloads Timer1, clears the interrupt flag and toggles the heartbeat LED.

The main loop retains responsibility for button debouncing and application state management.

No shared event variable is required because the heartbeat operation is short and bounded.

### Transition to PAUSED

1. Disable the Timer1 interrupt.
2. Stop Timer1.
3. Clear the pending interrupt flag.
4. Turn on the pilot LED.
5. Turn off the heartbeat LED.
6. Set the application state to PAUSED.

### Transition to RUNNING

1. Reload Timer1.
2. Clear the pending interrupt flag.
3. Enable the Timer1 interrupt.
4. Start Timer1.
5. Turn off the pilot LED.
6. Set the application state to RUNNING.

The interrupt masking sequence prevents Timer1 interrupt servicing after the mask takes effect during the pause transition.

## 4. Hardware Validation

### Test 01 — Steady-State Timing

| Measurement          | Expected | Measured |
| -------------------- | -------: | -------: |
| Frequency            |     1 Hz |  1.00 Hz |
| Period               |      1 s |  1.000 s |
| Positive pulse width |   500 ms | 500.0 ms |
| Duty cycle           |      50% |   50.00% |

**Result: PASS**

Measurements agree with nominal targets at the displayed instrument precision.

![Steady-state timing](evidence/interrupt_test_01.jpeg)

### Test 02 — Pause Transition

The pilot output transitions HIGH, the heartbeat output transitions LOW, and no subsequent heartbeat toggles occur within the captured paused interval.

The active heartbeat pulse is deliberately terminated upon entering PAUSED.

**Result: PASS**

![Pause transition](evidence/interrupt_test_02.jpeg)

### Test 03 — Resume Transition

The pilot output transitions LOW. The heartbeat remains LOW for approximately 500 ms before its first rising edge. The first positive pulse width measures 500.0 ms.

No premature heartbeat toggle is observed.

**Result: PASS**

![Resume transition](evidence/interrupt_test_03.jpeg)

### Test 04 — Button Functional Regression

| Test                        | Result |
| --------------------------- | ------ |
| Normal presses              | 10/10  |
| Short presses               | 5/5    |
| Long presses                | 5/5    |
| Total successful actuations | 20/20  |

**Result: PASS**

The intermittent button behavior observed during the original polling implementation was addressed through tactile switch replacement. No missed actuations occurred during the interrupt firmware regression test.

## 5. Engineering Design Defense

The following competencies were demonstrated:

- Independent timer configuration calculations.
- Timer resolution and quantization error analysis.
- Interrupt source identification and servicing.
- Interrupt initialization and enable-bit management.
- Race condition identification and mitigation.
- State-entry actions versus continuous state execution.
- Oscilloscope-based functional and timing validation.
- Git feature-branch development.

## 6. Conclusion

The Timer1 interrupt upgrade satisfies the defined educational and functional requirements. The heartbeat timing and application state transitions were successfully demonstrated on physical hardware.

This assessment is limited to the tested conditions and does not constitute production reliability certification.

**Final technical assessment: ACCEPTED.**
