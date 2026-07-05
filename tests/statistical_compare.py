"""Statistical comparison helpers for sample arrays (shared by bench and pytest)."""

from __future__ import annotations

import math
import struct
from pathlib import Path

import numpy as np
from scipy import stats


def read_binary(path: Path) -> tuple[str, np.ndarray]:
    data = path.read_bytes()
    tag = chr(data[0])
    (n,) = struct.unpack_from("<Q", data, 1)
    if tag == "i":
        arr = np.frombuffer(data, dtype=np.int32, offset=9, count=int(n))
    elif tag == "d":
        arr = np.frombuffer(data, dtype=np.float64, offset=9, count=int(n))
    else:
        raise ValueError(f"unknown tag {tag!r} in {path}")
    return tag, np.asarray(arr)


def check_discrete(a: np.ndarray, b: np.ndarray, alpha: float) -> tuple[bool, str]:
    if len(a) != len(b):
        return False, f"length mismatch {len(a)} vs {len(b)}"
    ma, mb = float(np.mean(a)), float(np.mean(b))
    va, vb = float(np.var(a)), float(np.var(b))
    se_mean = math.sqrt(max(va, vb) / len(a))
    if abs(ma - mb) > 3.0 * se_mean + 1e-9:
        return False, f"mean mismatch {ma:.6f} vs {mb:.6f}"
    se_var = math.sqrt(2.0 * max(va, vb) ** 2 / len(a))
    if abs(va - vb) > 3.0 * se_var + 1e-9:
        return False, f"variance mismatch {va:.6f} vs {vb:.6f}"
    vals = np.unique(np.concatenate([a, b]))
    obs_a = np.array([(a == v).sum() for v in vals], dtype=float)
    obs_b = np.array([(b == v).sum() for v in vals], dtype=float)
    exp = (obs_a + obs_b) / 2.0
    exp = np.maximum(exp, 1.0)
    chi_a = ((obs_a - exp) ** 2 / exp).sum()
    chi_b = ((obs_b - exp) ** 2 / exp).sum()
    df = max(len(vals) - 1, 1)
    crit = stats.chi2.ppf(1.0 - alpha, df)
    if chi_a > crit or chi_b > crit:
        return False, f"chi-square pmf mismatch chi_a={chi_a:.2f} chi_b={chi_b:.2f} crit={crit:.2f}"
    return True, "discrete moments+chi-square ok"


def check_continuous(a: np.ndarray, b: np.ndarray, alpha: float) -> tuple[bool, str]:
    if len(a) != len(b):
        return False, f"length mismatch {len(a)} vs {len(b)}"
    ks = stats.ks_2samp(a, b)
    if ks.pvalue < alpha:
        return False, f"KS p-value {ks.pvalue:.4g} < {alpha}"
    ma, mb = float(np.mean(a)), float(np.mean(b))
    va, vb = float(np.var(a)), float(np.var(b))
    se_mean = math.sqrt(max(va, vb) / len(a))
    if abs(ma - mb) > 3.0 * se_mean + 1e-9:
        return False, f"mean mismatch {ma:.6f} vs {mb:.6f}"
    return True, f"continuous KS ok p={ks.pvalue:.4g}"
