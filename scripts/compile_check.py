from __future__ import annotations

import shutil
import subprocess
import sys
from pathlib import Path


def main() -> int:
    repo_root = Path(__file__).resolve().parent.parent
    src_dir = repo_root / "src"
    config_path = src_dir / "config.h"
    template_path = src_dir / "config_template.h"

    created_temp_config = False
    if not config_path.exists():
        shutil.copyfile(template_path, config_path)
        created_temp_config = True
        print(f"created temporary config: {config_path}")
    else:
        print(f"using existing local config: {config_path}")

    try:
        result = subprocess.run(
            [sys.executable, "-m", "platformio", "run"],
            cwd=repo_root,
            check=False,
        )
        return result.returncode
    finally:
        if created_temp_config and config_path.exists():
            config_path.unlink()
            print(f"removed temporary config: {config_path}")


if __name__ == "__main__":
    raise SystemExit(main())
