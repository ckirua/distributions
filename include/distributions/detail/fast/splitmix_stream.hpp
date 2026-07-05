#pragma once

#include <cstddef>
#include <cstdint>

namespace distributions::detail::fast {

/// Lightweight counter-style stream for Tier-B trivial discrete samplers.
/// Faster per-draw than Philox; seeded independently of the serial `Pcg32` stream.
class SplitMix64Stream {
public:
    void seed(std::uint64_t seed_value) { state_ = seed_value; }

    [[nodiscard]] std::uint64_t next_u64() {
        state_ += 0x9e3779b97f4a7c15ULL;
        std::uint64_t z = state_;
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        return z ^ (z >> 31);
    }

    [[nodiscard]] double next_double() {
        return static_cast<double>(next_u64() >> 11U) * 0x1.0p-53;
    }

    void fill_uniform01(double* out, std::size_t n) {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = next_double();
        }
    }

private:
    std::uint64_t state_{0};
};

}  // namespace distributions::detail::fast
