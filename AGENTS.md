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
