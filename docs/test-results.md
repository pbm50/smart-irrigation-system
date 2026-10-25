# Test Results — Smart Irrigation System

## Version v0.1.0

**Platform:** Wokwi Simulator  
**Microcontroller:** ESP32 DevKit  
**Firmware:** `firmware/sketch.ino`  
**Hardware:** `hardware/wokwi/diagram.json`

## Testing Objective

Validate the irrigation controller's recovery detection and automatic cycle counter reset.

## Confirmed Tests

### TEST-001 — Automatic Recovery

**Initial condition:** An active irrigation session with accumulated irrigation cycles.

**Procedure:**
1. Increase simulated soil moisture to 40% or higher.
2. Maintain moisture at or above 40% for 30 seconds.
3. Observe the Serial Monitor.

**Expected result:**
- The controller enters the recovery verification state.
- The irrigation counter resets to 0/3 after successful recovery.
- The controller returns to monitoring.

**Result:** PASS

### TEST-002 — New Irrigation Session

**Initial condition:** TEST-001 completed successfully.

**Procedure:**
1. Reduce simulated moisture below 30%.
2. Observe the controller response and cycle counter.

**Expected result:**
- A new irrigation session begins.
- The irrigation counter starts at 1/3.
- The LED indicates pump activation.

**Result:** PASS

## Tests Pending

- Complete three-cycle safety lockout validation.
- Manual reset validation after lockout.
- Recovery interruption during the 30-second verification period.
- Additional timing and state-transition regression tests.
- Physical sensor calibration.
- Physical hardware validation.

## Notes

The potentiometer simulates soil moisture through an analog signal.

The LED represents the pump control output.

The moisture percentages are simulated ADC-derived values and do not represent calibrated physical soil moisture measurements.

## Conclusion

Two recovery-related test scenarios have been confirmed successfully in Wokwi.

The firmware is suitable for continued simulation development. Physical deployment requires additional testing, sensor calibration and appropriate pump-driving hardware.
