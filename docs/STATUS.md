# Status

## Summary

HiveTech is an ESP32 firmware prototype for live beehive monitoring. It reads BME280 sensor data, sends measurements over MQTT, supports OTA, and uses deep sleep for low-power operation.

## Current State

- Revival candidate with real firmware code, not just a stub
- Public again by explicit portfolio decision
- Still uses a legacy `config.h` include path
- PlatformIO/IoT profile context is now committed in `anr.profile.yaml` plus focused hardware and operations docs
- Compile-only validation now works via `python scripts/compile_check.py`
- CI build validation is defined in `.github/workflows/build.yml`
- Local developer deployment settings are still present outside committed templates

## Known Gaps

- The repo still relies on a simple local `config.h` include model
- Hardware wiring is documented only to the level visible in current firmware assumptions
- Deployment is still local and OTA-target specific
- MQTT contract is documented but still topic-only, without retained/availability semantics
- Real hardware smoke validation is still manual

## Next Milestone

Stabilize the revived public firmware baseline, then decide whether HiveTech becomes:

1. a standalone beehive telemetry node, or
2. a domain-specific branch of the broader SkySentinel ecosystem.
