# PIC16F1827 — Polling Firmware Hardware Validation Report

**Implementation:** Non-blocking super-loop with Timer0 and Timer1 polling  
**Initial capture date:** October 26, 2026  
**Corrective action and regression date:** November 15, 2026  
**Instrument:** Rigol DS1054Z, CH1/RA4 (yellow), CH2/RB1 (cyan)  
**Historical firmware checkpoint:** `59fb96a`

## 1. Purpose and Expected Behavior

A complete electrically valid RA4 press-and-release event should qualify both button edges and cause exactly one RUNNING/PAUSED transition. RB1 indicates PAUSED when HIGH and RUNNING when LOW, independently of the RB0 Timer1 heartbeat. The nominal Timer0 debounce threshold is 10 counts, approximately 10.24 ms.

## 2. Test Setup

- PIC16F1827 with an active-LOW RA4 onboard pushbutton; CH1 monitors the electrical input at the MCU.
- RB1 diagnostic output monitored on CH2. The application changes state on qualified release.
- RA4 falling-edge trigger; saved scope screenshots collected over USB with UltraSigma.
- Each trial consists of a physical press-and-release attempt. Saved screenshots are not full raw waveform records.

## 3. Initial Test Results

| ID  | Attempted transition | RA4 / RB1 observations                                                              | Classification                                                        | Capture                    |
| --- | -------------------- | ----------------------------------------------------------------------------------- | --------------------------------------------------------------------- | -------------------------- |
| 01  | RUNNING → PAUSED     | RA4 press-release; RB1 LOW → HIGH                                                   | Scope-correlated functional pass                                      | [01](evidence/test_01.png) |
| 02  | PAUSED → RUNNING     | RA4 press-release; RB1 HIGH → LOW                                                   | Scope-correlated functional pass                                      | [02](evidence/test_02.png) |
| 03  | RUNNING → PAUSED     | RA4 press-release; RB1 LOW → HIGH                                                   | Scope-correlated functional pass                                      | [03](evidence/test_03.png) |
| 04  | PAUSED → RUNNING     | Short/incomplete RA4 disturbance; RB1 remained HIGH                                 | No transition; suspected invalid electrical input                     | [04](evidence/test_04.png) |
| 05  | PAUSED → RUNNING     | Operator observed RB1 OFF and system RUNNING; transition not visible in saved frame | Operator-confirmed functional pass; waveform correlation inconclusive | [05](evidence/test_05.png) |
| 06  | RUNNING → PAUSED     | RA4 press-release; RB1 LOW → HIGH                                                   | Scope-correlated functional pass                                      | [06](evidence/test_06.png) |

The six trials comprise four independently scope-correlated transitions, one operator-confirmed transition with incomplete capture, and one missed physical actuation associated with an abnormal RA4 signal.

An earlier exploratory test recorded 17 successful operations and three misses in 20 physical attempts. Electrical validity was not independently classified for each attempt. These observations therefore do not establish a firmware-only reliability rate.

## 4. Failure Investigation

1. Intermittent missed button events occurred more often on quick presses than long presses.
2. An RB1 diagnostic LED was added to separate application-state changes from Timer1 heartbeat behavior.
3. Failed physical presses sometimes showed incomplete transitions or intermediate voltages on RA4 rather than a clean LOW. RB1 remained in its prior state.
4. In-circuit resistance readings across the four-terminal switch appeared inconsistent. These readings alone could not isolate an internal contact defect.
5. Cleaning the switch improved observed operation, although occasional misses persisted.

The most supported diagnosis was an intermittent switch contact or adjacent electrical-connection issue in the RA4 input path. The evidence did not isolate the exact physical failure mechanism. A valid RA4 waveform without a corresponding expected RB1 change would warrant renewed investigation of firmware input processing.

## 5. Firmware Corrections and Source Review

Source review identified and corrected missing rejection paths in the button state machine and tightened Timer1 overflow-flag handling on resume. The reviewed polling implementation includes:

- Failed press qualification returning to `BUTTON_WAIT_PRESS`.
- Failed release qualification returning to `BUTTON_WAIT_RELEASE`.
- RB1 HIGH in PAUSED and LOW in RUNNING.
- Timer1 counter reload and `TMR1IF` clearance on resume.
- A main loop without blocking delays.

The historical checkpoint `59fb96a` was recorded during the initial validation. Static source review does not independently establish a new build or associate every historical screenshot with an exact Git object hash.

## 6. Hardware Corrective Action and Regression — November 15, 2026

The tactile switch connected to RA4 was replaced to address the previously observed intermittent input behavior. Regression testing used the polling-based firmware.

| Test                  | Successful actuations |
| --------------------- | --------------------: |
| Normal button presses |                 10/10 |
| Short button presses  |                   5/5 |
| Long button presses   |                   5/5 |
| Total                 |                 20/20 |
| Missed actuations     |                     0 |

The previously observed intermittent behavior was not reproduced after switch replacement. The result establishes a functional hardware baseline for the subsequent interrupt-driven implementation under the recorded test conditions.

## 7. Conclusions and Limitations

The polling firmware demonstrated the intended RUNNING/PAUSED behavior under observable valid-input conditions. Initial missed events were associated with abnormal electrical input, and the symptom was not reproduced in the 20-actuation post-replacement regression.

These finite functional tests do not certify production reliability, rule out all firmware defects, or prove the precise physical failure mechanism. Test 05 remains operator-confirmed rather than independently waveform-correlated.

The subsequent [Timer1 interrupt-driven implementation](https://github.com/bob7185/Embedded_Foundations_Pack01/tree/feature/timer1-interrupt) is documented in its own [hardware validation report](https://github.com/bob7185/Embedded_Foundations_Pack01/blob/feature/timer1-interrupt/docs/INTERRUPT_VALIDATION_REPORT.md).
