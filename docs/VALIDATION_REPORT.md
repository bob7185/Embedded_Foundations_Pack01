# Pack 01 — Final Hardware Validation Report

**Project:** PIC16F1827 Embedded Foundations Pack 01  
**Capture date:** October 26, 2026  
**Instrument:** Rigol DS1054Z, CH1/RA4 (yellow), CH2/RB1 (cyan)  
**Firmware checkpoint:** `59fb96a` reported pushed to `main`; final source independently reviewed from the uploaded `main.c`  
**Assessment:** Foundation objectives demonstrated; suspected hardware-input fault documented.

## Purpose and expected behavior

On a complete electrically valid RA4 press/release, the wait-then-sample debouncer should qualify press and release, causing exactly one `RUNNING`/`PAUSED` change. RB1 indicates PAUSED when HIGH and RUNNING when LOW, independently of the RB0 Timer1 heartbeat. The nominal Timer0 debounce threshold is 10 counts, approximately 10.24 ms.

## Test setup

- PIC16F1827 with active-LOW RA4 board pushbutton; CH1 observes the input at the MCU.
- RB1 diagnostic indicator connected to CH2. Transition is made on qualified release.
- RA4 falling-edge trigger used; saved screen images collected over USB with UltraSigma.
- Each trial was a physical press-and-release attempt. Photos/scope screenshots are not substitutes for full raw waveform samples; classification below preserves those limitations.

## Results

| ID  | Attempted transition       | RA4 / RB1 observations                                                                  | Classification                                                        | Capture                    |
| --- | -------------------------- | --------------------------------------------------------------------------------------- | --------------------------------------------------------------------- | -------------------------- |
| 01  | RUNNING → PAUSED           | RA4 press-release; RB1 LOW → HIGH                                                       | Scope-correlated functional pass                                      | [01](evidence/test_01.png) |
| 02  | PAUSED → RUNNING           | RA4 press-release; RB1 HIGH → LOW                                                       | Scope-correlated functional pass                                      | [02](evidence/test_02.png) |
| 03  | RUNNING → PAUSED           | RA4 press-release; RB1 LOW → HIGH                                                       | Scope-correlated functional pass                                      | [03](evidence/test_03.png) |
| 04  | PAUSED → RUNNING attempted | Short/incomplete RA4 disturbance; RB1 stayed HIGH                                       | No system transition; suspected invalid electrical input              | [04](evidence/test_04.png) |
| 05  | PAUSED → RUNNING           | Operator observed RB1 LED OFF and system RUNNING; transition not visible in saved frame | Functional pass by operator report; waveform correlation inconclusive | [05](evidence/test_05.png) |
| 06  | RUNNING → PAUSED           | RA4 press-release; RB1 LOW → HIGH                                                       | Scope-correlated functional pass                                      | [06](evidence/test_06.png) |

**Count:** Four independently scope-correlated transitions, one operator-confirmed transition with incomplete capture, and one missed physical actuation correlated with an abnormal RA4 signal. This is **not** a statistically powered reliability study or a quantified firmware pass rate.

Earlier exploratory test: 17 successful actuations and 3 missed of 20 physical attempts. Electrical validity was not classified for each earlier actuation, so no independent firmware reliability percentage follows from that result.

## Failure investigation

1. Intermittent missed button events occurred more often on quick presses than long presses.
2. An RB1 diagnostic LED was added to separate the software state change from Timer1 heartbeat output.
3. With a failed physical press, CH1 sometimes showed incomplete transitions or intermediate voltages rather than a clean RA4 LOW. RB1 also remained in its prior state.
4. In-circuit resistance readings across the four-terminal switch appeared inconsistent. Because the switch was measured in-circuit, these readings alone cannot isolate an internal contact defect.
5. Cleaning the switch improved observed operation, though occasional misses persisted.

**Most supported diagnosis:** Intermittent switch/contact or adjacent electrical-connection issue in the RA4 input path. Hardware replacement/repair is a reasonable future maintenance action; no further switch probing was necessary to close the foundation exercise. A valid RA4 waveform with no corresponding RB1 transition would warrant reopening firmware/input-processing review.

## Source review and design defense

The latest supplied `main.c` confirms: correct failed-press rejection to `BUTTON_WAIT_PRESS`; correct failed-release rejection to `BUTTON_WAIT_RELEASE`; RB1 HIGH for PAUSED and LOW for RUNNING; Timer1 reloaded and `TMR1IF` cleared on resume; no blocking delay in the main loop. The engineer completed an oral design defense explaining timer selection, nonblocking polling, four-state debounce and oscilloscope-based fault isolation. **Design defense passed with minor technical-language corrections.**

The current source review is static and does not itself demonstrate a new build or independently tie every screenshot to the exact Git object hash. The screenshot of the Git terminal confirms a push of `59fb96a` as reported during the session.

## Conclusion and closeout decision

**Engineering Pack 01 — ACCEPTED for its defined educational foundation scope, with a documented hardware limitation.** The required behavior was implemented and demonstrated under observable valid-input conditions; remaining missed actuations are best explained by intermittent RA4 electrical input, not proved to be software failures. This assessment does **not** certify production reliability, eliminate every firmware defect or prove the exact physical component failure.

Further bench debugging is out of scope unless contradictory evidence emerges. Recommended future board maintenance: replace or repair the suspect tactile switch/connection, then retest if reusing the board for a higher-reliability application.

## Hardware Repair and Regression Test — November 15, 2026

The tactile switch connected to RA4 was replaced to address the previously observed intermittent input behavior.

Regression testing was performed using the existing polling-based firmware.

| Test                  | Result |
| --------------------- | ------ |
| Normal button presses | 10/10  |
| Short button presses  | 5/5    |
| Long button presses   | 5/5    |
| Missed actuations     | 0      |

**Conclusion:** All 20 button actuations passed. The previously observed intermittent behavior was not reproduced following switch replacement. The documented hardware limitation is considered resolved within the scope of this regression test.

This establishes the functional hardware baseline for the upcoming Timer1 interrupt implementation.
