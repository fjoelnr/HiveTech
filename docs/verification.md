# Verification

## Goal

Keep firmware changes reproducible and separate "compiles locally" from "works on the hive node".

## Minimum Checks

1. Copy `src/config_template.h` to local `src/config.h` or provide an equivalent local config file.
2. Run `platformio run` for `env:esp32dev`.
3. Flash or OTA-update the node using the local deployment path from `platformio.ini`.
4. Confirm serial boot output reaches:
   - Wi-Fi connect
   - MQTT connect
   - BME280 detection
   - deep sleep entry
5. Verify these MQTT topics receive fresh values:
   - `HT_BME280_Temperature`
   - `HT_BME280_Humidity`
   - `HT_BME280_Pressure`
   - `HT_BME280_Altitude`

## Separation Of Checks

- Compile-only validation is useful for refactors and dependency changes.
- Real hardware validation is still mandatory after sensor, networking, OTA, or sleep-logic changes.
