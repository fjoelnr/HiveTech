# AGENTS.md

## Project Intent

HiveTech is the beehive monitoring firmware line in the portfolio. Treat it as a revival project, not a legacy archive.

## Working Rules

- Preserve local hardware-specific configuration and deployment settings.
- Do not commit `src/config.h` or other machine-specific secrets.
- Prefer incremental refactors over broad rewrites until the hardware scope is stable.
- Keep MQTT topic changes explicit and documented because they affect downstream automations.

## Current Architecture

- `src/main.cpp`: boot flow, Wi-Fi, MQTT, BME280 readout, deep sleep
- `src/HiveTech.h`: shared includes, globals, and constants
- `src/config_template.h`: public template for local runtime configuration

## Delivery Flow

Use `feature -> develop -> main`. Keep public-facing docs and topics aligned with the actual firmware scope.

<!-- ANR PROFILE START -->
## Active Stack Profile

This repository uses the `platformio-iot` stack profile.

### Additional Focus

- PlatformIO build and upload configuration
- board, sensor, and pin assumptions
- telemetry/topic contracts
- hardware validation beyond compile success

### Additional Working Rules

- never commit device-specific secrets or local deployment values
- keep hardware assumptions explicit in docs
- treat topic and payload changes as interface changes
- separate build success from real-device verification success
- keep the local `platformio.ini` upload target as developer-local state unless a shared deployment path is intentionally introduced
<!-- ANR PROFILE END -->

