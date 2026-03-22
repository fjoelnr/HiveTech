# Verification

## Goal

Keep firmware changes reproducible and separate "compiles locally" from "works on the hive node".

## Minimum Checks

1. Run `python scripts/compile_check.py` for a compile-only check.
2. Run `python scripts/hardware_smoke_check.py` before OTA upload.
3. If you need device-specific validation, provide a real local `src/config.h`.
4. Flash or OTA-update the node using the local deployment path from `platformio.ini`.
5. Confirm serial boot output reaches:
   - Wi-Fi connect
   - MQTT connect
   - BME280 detection
   - deep sleep entry
6. Verify these MQTT topics receive fresh values:
   - `HT_BME280_Temperature`
   - `HT_BME280_Humidity`
   - `HT_BME280_Pressure`
   - `HT_BME280_Altitude`

## Separation Of Checks

- Compile-only validation is now available via `python scripts/compile_check.py`.
- Hardware preflight is now available via `python scripts/hardware_smoke_check.py`.
- Real hardware validation is still mandatory after sensor, networking, OTA, or sleep-logic changes.
