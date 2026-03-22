from __future__ import annotations

import configparser
import re
import socket
import sys
from dataclasses import dataclass
from pathlib import Path


PLACEHOLDER_VALUES = {"your_wifi_ssid", "your_wifi_password", "your_mqtt_server"}


@dataclass
class CheckResult:
    name: str
    ok: bool
    detail: str


def _load_platformio_env(repo_root: Path) -> tuple[str, dict[str, str]]:
    config = configparser.ConfigParser(inline_comment_prefixes=(";", "#"))
    config.read(repo_root / "platformio.ini", encoding="utf-8")
    env_sections = [section for section in config.sections() if section.startswith("env:")]
    if not env_sections:
        raise ValueError("platformio.ini does not define an [env:*] section")
    section = env_sections[0]
    return section, {key: value.strip() for key, value in config[section].items()}


def _read_config_values(config_path: Path) -> dict[str, str]:
    content = config_path.read_text(encoding="utf-8")
    values: dict[str, str] = {}
    for key in ("ssid", "password", "mqtt_server"):
        match = re.search(rf'{key}\s*=\s*"([^"]+)"', content)
        if match:
            values[key] = match.group(1).strip()
    return values


def _is_serial_target(value: str) -> bool:
    upper = value.upper()
    return upper.startswith("COM") or value.startswith("/dev/")


def _check_host(host: str, port: int | None = None) -> CheckResult:
    try:
        resolved = socket.gethostbyname(host)
    except OSError as exc:
        return CheckResult(f"resolve:{host}", False, f"name resolution failed: {exc}")

    if port is None:
        return CheckResult(f"resolve:{host}", True, f"resolved to {resolved}")

    try:
        with socket.create_connection((host, port), timeout=2):
            return CheckResult(f"connect:{host}:{port}", True, f"reachable at {resolved}:{port}")
    except OSError as exc:
        return CheckResult(f"connect:{host}:{port}", False, f"connection failed: {exc}")


def main() -> int:
    repo_root = Path(__file__).resolve().parent.parent
    config_path = repo_root / "src" / "config.h"

    results: list[CheckResult] = []

    try:
        env_name, env_config = _load_platformio_env(repo_root)
    except ValueError as exc:
        print(f"preflight error: {exc}")
        return 1

    upload_target = env_config.get("upload_port", "")
    upload_protocol = env_config.get("upload_protocol", "")

    results.append(CheckResult("platformio-env", True, f"using {env_name}"))
    results.append(CheckResult("upload-protocol", bool(upload_protocol), f"value={upload_protocol or 'missing'}"))

    if not config_path.exists():
        results.append(CheckResult("config.h", False, f"missing local config: {config_path}"))
    else:
        values = _read_config_values(config_path)
        missing_keys = [key for key in ("ssid", "password", "mqtt_server") if key not in values]
        if missing_keys:
            results.append(CheckResult("config.h", False, f"missing keys: {', '.join(missing_keys)}"))
        else:
            placeholders = [key for key, value in values.items() if value in PLACEHOLDER_VALUES]
            if placeholders:
                results.append(CheckResult("config.h", False, f"placeholder values still set: {', '.join(placeholders)}"))
            else:
                results.append(CheckResult("config.h", True, "real local credentials/config detected"))

            mqtt_host = values.get("mqtt_server", "")
            if mqtt_host:
                results.append(_check_host(mqtt_host, 1883))

    if not upload_target:
        results.append(CheckResult("upload-target", False, "upload_port missing"))
    elif _is_serial_target(upload_target):
        results.append(CheckResult("upload-target", True, f"serial target configured: {upload_target}"))
    else:
        results.append(_check_host(upload_target, 3232))

    failures = 0
    print("HiveTech hardware preflight")
    print("")
    for result in results:
        status = "PASS" if result.ok else "FAIL"
        print(f"[{status}] {result.name}: {result.detail}")
        if not result.ok:
            failures += 1

    print("")
    if failures:
        print(f"preflight result: FAIL ({failures} failed checks)")
        return 1

    print("preflight result: PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
