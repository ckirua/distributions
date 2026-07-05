#!/usr/bin/env python3
"""Entry point for library codegen. Implementation lives in tools/codegen/."""

from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from codegen.main import main

if __name__ == "__main__":
    main()
