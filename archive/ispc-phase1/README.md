# Archived Phase-1 ISPC kernels (removed from build in optimize batch 9)

These sources were an early SIMD experiment. `sample_ispc()` in the bench harness always fell back to C++ and was never wired into production samplers.

The v0.3.0 optimization track uses `include/distributions/detail/fast/` (Tier-B counter-RNG / SplitMix64 paths) instead.

Do not revive this directory as the primary SIMD strategy. Kept for historical reference only.
