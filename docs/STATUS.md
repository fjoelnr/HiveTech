# Status

## Summary

HiveTech is an ESP32 firmware prototype for live beehive monitoring. It reads BME280 sensor data, sends measurements over MQTT, supports OTA, and uses deep sleep for low-power operation.

## Current State

- Revival candidate with real firmware code, not just a stub
- Public again by explicit portfolio decision
- Still uses a legacy `config.h` include path
- No modern CI/build validation yet
- Local developer deployment settings are still present outside committed templates

## Known Gaps

- No committed configuration template existed before this cleanup pass
- No README or public-facing operational context existed
- Hardware wiring and deployment procedure are not yet documented
- MQTT contract is still implicit in code

## Next Milestone

Stabilize the public repo baseline, then decide whether HiveTech becomes:

1. a standalone beehive telemetry node, or
2. a domain-specific branch of the broader SkySentinel ecosystem.
