# Polling Firmware — Hardware Validation and Fault Investigation

**Target:** PIC16F1827  
**Implementation:** Polling-based heartbeat and button state machine (`main`)  
**Initial capture date:** October 26, 2026  
**Corrective action and regression:** November 15, 2026  
**Instrument:** Rigol DS1054Z  
**Source reference:** [`59fb96a`](https://github.com/bob7185/Embedded_Foundations_Pack01/commit/59fb96a) (original debounce correction); polling branch reviewed at `26bd845`.

## 1. Purpose and Expected Behavior

Verify that a complete, electrically valid RA4 press-and-release event is qualified by the wait-then-sample debouncer and produces one RUNNING/PAUSED transition. RB1 is HIGH in PAUSED and LOW in RUNNING, independently of the RB0 Timer1 heartbeat. Timer0's nominal debounce threshold is 10 counts (approximately 10.24 ms, excluding software overhead).

## 2. Test Setup

- PIC16F1827 development board with active-LOW pushbutton on RA4.
- CH1 (yellow): RA4 at the microcontroller input.
- CH2 (cyan): RB1 diagnostic output, which changes on qualified release.
- RA4 falling-edge trigger; saved screenshots acquired over USB using Rigol UltraSigma.
- Each trial consisted of a physical press-and-release attempt. Saved screenshots are not full raw waveform records; classifications reflect only the available observations.

## 3. Initial Results — October 26, 2026

| ID | Attempted transition | RA4 / RB1 observations | Classification | Evidence |
| --- | --- | --- | --- | --- |
| 01 | RUNNING → PAUSED | RA4 press-release; RB1 LOW → HIGH | Scope-correlated functional pass | [01](evidence/test_01.png) |
| 02 | PAUSED → RUNNING | RA4 press-release; RB1 HIGH → LOW | Scope-correlated functional pass | [02](evidence/test_02.png) |
| 03 | RUNNING → PAUSED | RA4 press-release; RB1 LOW → HIGH | Scope-correlated functional pass | [03](evidence/test_03.png) |
| 04 | PAUSED → RUNNING attempted | Short/incomplete RA4 disturbance; RB1 remained HIGH | No transition; suspected invalid electrical input | [04](evidence/test_04.png) |
| 05 | PAUSED → RUNNING | Operator observed RB1 OFF and system RUNNING; transition not visible in saved frame | Operator-confirmed functional pass; scope correlation inconclusive | [05](evidence/test_05.png) |
| 06 | RUNNING → PAUSED | RA4 press-release; RB1 LOW → HIGH | Scope-correlated functional pass | [06](evidence/test_06.png) |

Four transitions were independently correlated with the scope traces; one was confirmed by direct observation but not independently shown in the saved frame. One physical actuation failed to produce a state change and coincided with abnormal RA4 input.

An earlier exploratory check recorded 17 successful actuations and three misses out of 20 physical attempts. Electrical validity was not classified for each attempt, so these counts do not establish a firmware-only pass rate.

## 4. Failure Investigation

1. Missed button events occurred intermittently, more often with quick presses than long presses.
2. Firmware review identified missing rejection paths in the press/release state machine. The control flow was corrected; Timer1 overflow-flag handling on resume was also tightened.
3. RB1 was added as an independent state indicator to distinguish accepted software events from heartbeat output timing.
4. During some unsuccessful physical actuations, CH1 showed incomplete transitions or intermediate voltages rather than a clean RA4 LOW. RB1 remained in its previous state.
5. In-circuit resistance readings across the four-terminal switch were inconsistent. These readings alone do not isolate an internal contact defect.
6. Cleaning the switch improved observed operation, although intermittent misses persisted.

**Diagnosis:** The combined evidence supports an intermittent tactile-switch contact or adjacent electrical-connection issue in the RA4 input path. It does not establish the exact internal failure mechanism. A valid RA4 waveform with no corresponding RB1 transition would warrant further firmware review.

## 5. Implementation Review

The polling firmware implements failed-press rejection to `BUTTON_WAIT_PRESS` and failed-release rejection to `BUTTON_WAIT_RELEASE`. RB1 tracks application state independently of the heartbeat. Timer1 is reloaded and its overflow flag cleared before restart; the PAUSED path stops the timer, clears the flag and holds RB0 LOW. No blocking delays are used in the main loop.

This static review documents the implementation's control flow. It is not an independent build or a cryptographic association of each historical capture with a particular binary.

## 6. Hardware Repair and Regression — November 15, 2026

The tactile switch connected to RA4 was replaced to address the intermittent input behavior. Regression testing was then performed with the existing polling-based firmware.

| Physical actuation | Successful / attempted |
| --- | ---: |
| Normal presses | 10/10 |
| Short presses | 5/5 |
| Long presses | 5/5 |
| **Total** | **20/20** |

**Missed actuations:** 0. The result was recorded by direct bench observation. The previously observed symptom did not recur during the regression run.

## 7. Conclusion

The polling firmware demonstrated the required RUNNING/PAUSED behavior with valid electrical input. Historical missed actuations were associated with an abnormal RA4 input, and the symptom was not reproduced in the 20-actuation regression after replacement of the tactile switch. The repaired hardware provides the functional baseline for the Timer1 interrupt revision.

The findings are limited to the observed test conditions. They do not establish a statistical lifetime reliability rate, prove an exact component-level failure mechanism or certify production readiness. The original captures are preserved to document the investigation and corrective action.
