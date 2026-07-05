#!/usr/bin/env bash
# Resume batch 1 codegen bench sweep if incomplete.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"
TARGET=171
count="$(find results/codegen-tier-a -maxdepth 1 -name '*.csv' 2>/dev/null | wc -l)"
if pgrep -f "bench_codegen.py --sweep" >/dev/null 2>&1; then
  echo "sweep already running ($count/$TARGET csvs)"
  exit 0
fi
if [ "$count" -ge "$TARGET" ]; then
  echo "sweep complete ($count csvs)"
  exit 0
fi
echo "resuming sweep ($count/$TARGET)"
make bench-codegen-sweep-resume
