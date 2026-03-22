# Operations

## Runtime

HiveTech is a local-first firmware repo.

Current runtime flow:

1. local `src/config.h` provides Wi-Fi SSID, password, and MQTT broker
2. firmware boots on ESP32
3. node connects to Wi-Fi
4. OTA is initialized
5. BME280 values are read and published to MQTT
6. node returns to timed deep sleep

`platformio.ini` currently contains a developer-local `upload_port`. Treat that as local machine state, not as committed team-wide truth.

## Build And Validation

- `python scripts/compile_check.py` is the default compile-only validation path
- the script creates a temporary `src/config.h` from `src/config_template.h` when needed
- CI uses the same path so local and remote validation stay aligned

## Safety Rules

- separate local credentials from committed templates
- keep `src/config.h` untracked
- verify the target IP/device before OTA uploads
- document any destructive or risky deployment step explicitly

## Recovery Notes

- If Wi-Fi or MQTT details change, update only local config files.
- If OTA fails, fall back to a direct flash path rather than repeatedly retrying against the wrong node.
- If BME280 detection fails, check wiring and I2C address assumptions before changing code.
