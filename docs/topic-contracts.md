# Topic Contracts

## Purpose

Document the MQTT topics currently published by HiveTech.

## Topics

- `HT_BME280_Temperature`
  Payload: numeric value as text
  Meaning: ambient temperature from the BME280 in degrees Celsius
- `HT_BME280_Humidity`
  Payload: numeric value as text
  Meaning: relative humidity from the BME280
- `HT_BME280_Pressure`
  Payload: numeric value as text
  Meaning: barometric pressure from the BME280 divided by `100`
- `HT_BME280_Altitude`
  Payload: numeric value as text
  Meaning: altitude reported by the BME280 library

Current code publishes these topics once per wake cycle after sensor readout and MQTT reconnect.

## Change Rules

- Keep topic names stable unless downstream automations are migrated deliberately.
- Treat unit changes or payload-format changes as interface changes.
- If retained messages, availability topics, or structured payloads are introduced, document them here before rollout.
