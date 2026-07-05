// Per-lane PRNG for SPMD foreach (statistically equivalent batch; not bit-identical to serial PCG).
inline double splitmix_double(uniform unsigned int64 seed, varying int index) {
    varying unsigned int64 x =
        seed + 0x9E3779B97F4A7C15ULL * (varying unsigned int64)(index + 1);
    x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ULL;
    x = (x ^ (x >> 27)) * 0x94D049BB133111EBULL;
    x = x ^ (x >> 31);
    return (double)x / (double)0xFFFFFFFFFFFFFFFFULL;
}
