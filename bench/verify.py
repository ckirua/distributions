#!/usr/bin/env python3
"""Statistical equivalence checker for distribution sample outputs."""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tests"))

from statistical_compare import check_continuous, check_discrete, read_binary  # noqa: E402


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("a", type=Path)
    parser.add_argument("b", type=Path)
    parser.add_argument("--alpha", type=float, default=0.01)
    args = parser.parse_args()

    tag_a, arr_a = read_binary(args.a)
    tag_b, arr_b = read_binary(args.b)
    if tag_a != tag_b:
        print(f"FAIL: dtype tag mismatch {tag_a} vs {tag_b}", file=sys.stderr)
        return 1

    if tag_a == "i":
        ok, msg = check_discrete(arr_a, arr_b, args.alpha)
    else:
        ok, msg = check_continuous(arr_a, arr_b, args.alpha)

    if ok:
        print(msg)
        return 0
    print(f"FAIL: {msg}", file=sys.stderr)
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
