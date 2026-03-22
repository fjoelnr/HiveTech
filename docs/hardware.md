# Hardware

## Target

HiveTech currently targets a generic `esp32dev` board definition with a single BME280 environmental sensor.

Current firmware assumptions from the code:

- board: ESP32 dev board (`[env:esp32dev]` in `platformio.ini`)
- primary sensor: BME280 via I2C
- I2C address: `0x77`
- sleep cycle: `TIME_TO_SLEEP = 60` seconds
- deep sleep wake source: timer wakeup

The code calls `Wire.begin()` without explicit pins, so the active SDA/SCL mapping depends on the actual ESP32 board defaults or local hardware wiring.

## Pin Map

- `GPIO25`: `redLEDpin`
- `GPIO4`: `greenLEDpin`
- `GPIO15`: `ONE_WIRE_BUS`
- `GPIO5`: `SD_CS`

Not every declared pin is currently exercised in the active telemetry flow. Treat this list as the current code-level hardware contract, not as proof that every peripheral is physically assembled on the node.

## Notes

- The firmware currently uses only BME280 telemetry in the active publish path.
- OTA is enabled, so the deployed node must stay reachable on the local network during maintenance.
- Before adding more hive-specific sensors, document their wiring and topic outputs here first.
