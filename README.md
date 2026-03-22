# HiveTech

ESP32-based beehive monitoring firmware for live environmental telemetry.

## What It Is

HiveTech is an early firmware project for monitoring beehive conditions with an ESP32, a BME280 sensor, deep sleep, OTA updates, and MQTT publishing. The current code publishes temperature, humidity, pressure, and altitude values to MQTT topics and is intended as the technical base for a revived live beehive monitoring project.

## Current Scope

- ESP32 + BME280 environmental telemetry
- MQTT publishing for downstream automation and dashboards
- OTA update support
- Deep-sleep based low-power operation
- Foundation for future hive-specific monitoring extensions

## Status

This repository is being revived from an older prototype. The current firmware is functional as a starting point, but the repo still needs cleanup and modernization. The public baseline now includes documentation, ownership, and configuration templates so work can continue in a controlled way.

See [docs/STATUS.md](docs/STATUS.md) for the current project state.

## Configuration

The firmware expects a local `src/config.h` file that is not committed. Start from [src/config_template.h](src/config_template.h) and adjust the values for your network and MQTT broker.

For compile-only validation, the repository now supports a temporary-config path:

```bash
python scripts/compile_check.py
```

If `src/config.h` is missing, the script copies `src/config_template.h`, runs `platformio run`, and removes the temporary file again.

Additional operating context now lives in:

- [docs/hardware.md](docs/hardware.md)
- [docs/operations.md](docs/operations.md)
- [docs/topic-contracts.md](docs/topic-contracts.md)
- [docs/verification.md](docs/verification.md)
- [anr.profile.yaml](anr.profile.yaml)

## Topics Published

- `HT_BME280_Temperature`
- `HT_BME280_Humidity`
- `HT_BME280_Pressure`
- `HT_BME280_Altitude`

## Immediate Next Steps

1. Replace the legacy `config.h` flow with a clearer credentials/config separation.
2. Add a real hardware smoke-test routine after the new compile-only validation path.
3. Decide which parts should remain standalone versus moving into `SkySentinel`.
4. Add hive-specific telemetry beyond the current BME280 baseline once the hardware scope is stable.
