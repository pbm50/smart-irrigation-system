# Smart Irrigation System

ESP32-based smart irrigation system for automated plant watering.

## Project Overview

This project aims to develop a low-cost, modular irrigation controller capable of monitoring soil moisture and activating a water pump automatically.

The long-term goal is to integrate IoT connectivity, n8n automation, environmental monitoring and multiple irrigation zones.

## Current Version

**v0.1.0 — Initial simulated prototype**

The current prototype has been developed and tested in Wokwi using an ESP32 microcontroller.

## Features

- Simulated soil moisture monitoring using an analog potentiometer.
- Automatic irrigation activation below a 30% simulated moisture threshold.
- Maximum irrigation duration of 5 seconds per cycle.
- 15-second waiting period between irrigation cycles.
- Maximum of 3 cycles per irrigation session.
- Automatic recovery detection at 40% simulated moisture.
- Recovery confirmation after 30 seconds of stable moisture.
- Automatic cycle counter reset after successful recovery.
- Manual reset using a pushbutton.
- Safety lockout after 3 unsuccessful irrigation cycles.
- Non-blocking timing using `millis()`.

## Hardware — Wokwi Simulation

| Component | ESP32 Pin |
|---|---|
| Potentiometer signal | GPIO 34 |
| LED pump indicator | GPIO 26 |
| Pushbutton | GPIO 27 |
| LED resistor | 220 Ω |

The potentiometer represents the soil moisture sensor, while the LED represents the pump control signal.

## Repository Structure

- `firmware/sketch.ino` — ESP32 firmware.
- `hardware/wokwi/diagram.json` — Wokwi circuit diagram.
- `docs/` — Architecture, testing and version history.

## Important Notes

The current moisture percentage is derived directly from the ESP32 ADC reading and has not been calibrated against a physical soil moisture sensor.

The LED represents the control signal only. A physical pump will require a suitable driver circuit and power supply.

## Roadmap

- v0.1.0 — Initial simulated irrigation controller.
- v0.2.0 — Wi-Fi connectivity and telemetry.
- Future versions — n8n integration, calibrated sensors, real pump control, monitoring dashboard and multiple irrigation zones.

## Development Environment

- ESP32
- Arduino C++
- Wokwi
- GitHub

## Project Status

Initial simulation prototype. Recovery and counter-reset scenarios validated. Additional regression tests and physical hardware validation pending.
