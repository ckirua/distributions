#pragma once

#include <cstdint>

namespace distributions {

/// PCG32 — one engine per distribution object; batch and single-draw share state.
class Pcg32 {
public:
    Pcg32() = default;

    explicit Pcg32(std::uint64_t seed_value) { seed(seed_value); }

    void seed(std::uint64_t seed_value) {
        state_ = 0;
        inc_ = (seed_value << 1u) | 1u;
        (void)next_u32();
        state_ += seed_value;
        (void)next_u32();
    }

    [[nodiscard]] std::uint32_t next_u32() {
        const std::uint64_t old = state_;
        state_ = old * 6364136223846793005ULL + inc_;
        const std::uint32_t xorshifted =
            static_cast<std::uint32_t>(((old >> 18u) ^ old) >> 27u);
        const std::uint32_t rot = static_cast<std::uint32_t>(old >> 59u);
        return (xorshifted >> rot) | (xorshifted << ((-static_cast<int>(rot)) & 31));
    }

    /// Uniform double in [0, 1).
    [[nodiscard]] double next_double() {
        return static_cast<double>(next_u32() >> 8) * 0x1.0p-24;
    }

private:
    std::uint64_t state_{0x853c49e6748fea9bULL};
    std::uint64_t inc_{0xda3e39cb94b95bdbULL};
};

}  // namespace distributions
