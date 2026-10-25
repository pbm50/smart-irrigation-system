# Changelog

All notable changes to the Smart Irrigation System will be documented in this file.

## [0.1.0] - 2026-10-24

### Added
- Initial ESP32 irrigation controller.
- Analog moisture simulation.
- LED-based pump indication.
- Automatic irrigation control.
- Irrigation state machine.
- Three-cycle safety limit.
- Manual reset button.
- Moisture hysteresis.
- Automatic recovery confirmation.
- Automatic irrigation counter reset.
- Serial monitoring and event messages.

### Validation
- Confirmed counter reset after stable moisture recovery.
- Confirmed new irrigation session starts at cycle 1/3.

### Limitations
- Simulation only.
- No physical sensor calibration.
- No physical pump or driver.
- No Wi-Fi telemetry.
- Additional regression testing pending.
