#pragma once

#if defined(DISTRIBUTIONS_HAS_AVX2)

#include <cstdint>
#include <immintrin.h>

namespace distributions::detail::simd {

/// Philox4x32-10 — same algorithm as `detail::CounterRng`; outputs converted with AVX2.
class Philox4x32Avx2 {
public:
    void seed(std::uint64_t seed_value, std::uint32_t stream_id = 0) {
        std::uint64_t sm = seed_value ^ (static_cast<std::uint64_t>(stream_id) << 32);
        key_[0] = splitmix32(sm);
        key_[1] = splitmix32(sm);
        counter_[0] = 0;
        counter_[1] = 0;
        counter_[2] = stream_id;
        counter_[3] = 0;
        lane_ = 4;
    }

    [[nodiscard]] double next_double() {
        if (lane_ >= 4) {
            generate_block();
            lane_ = 0;
        }
        return u01_scalar(block_[lane_++]);
    }

    /// Four [0, 1) uniforms from one Philox block (matches scalar block order).
    [[nodiscard]] __m256d generate_u01_block() {
        generate_block();
        lane_ = 4;
        return u32x4_to_u01_avx2(block_[0], block_[1], block_[2], block_[3]);
    }

private:
    static constexpr std::uint32_t kPhiloxM0 = 0xD2511F53U;
    static constexpr std::uint32_t kPhiloxM1 = 0xCD9E8D57U;
    static constexpr std::uint32_t kPhiloxW0 = 0x9E3779B9U;
    static constexpr std::uint32_t kPhiloxW1 = 0xBB67AE85U;

    static std::uint32_t splitmix32(std::uint64_t& state) {
        state += 0x9e3779b97f4a7c15ULL;
        std::uint64_t z = state;
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        return static_cast<std::uint32_t>(z ^ (z >> 31));
    }

    static std::uint32_t mulhi(std::uint32_t a, std::uint32_t b) {
        return static_cast<std::uint32_t>((static_cast<std::uint64_t>(a) * b) >> 32U);
    }

    static std::uint32_t mullo(std::uint32_t a, std::uint32_t b) {
        return static_cast<std::uint32_t>(static_cast<std::uint64_t>(a) * b);
    }

    static double u01_scalar(std::uint32_t w) {
        return static_cast<double>(w >> 8U) * 0x1.0p-24;
    }

    static __m256d u32x4_to_u01_avx2(
        std::uint32_t w0, std::uint32_t w1, std::uint32_t w2, std::uint32_t w3) {
        const __m256d scale = _mm256_set1_pd(0x1.0p-24);
        __m256d v = _mm256_set_pd(
            static_cast<double>(w3 >> 8U),
            static_cast<double>(w2 >> 8U),
            static_cast<double>(w1 >> 8U),
            static_cast<double>(w0 >> 8U));
        return _mm256_mul_pd(v, scale);
    }

    void philox_round(std::uint32_t ctr[4], const std::uint32_t key[2]) {
        const std::uint32_t hi0 = mulhi(kPhiloxM0, ctr[0]);
        const std::uint32_t lo0 = mullo(kPhiloxM0, ctr[0]);
        const std::uint32_t hi1 = mulhi(kPhiloxM1, ctr[2]);
        const std::uint32_t lo1 = mullo(kPhiloxM1, ctr[2]);
        ctr[0] = hi1 ^ ctr[1] ^ key[0];
        ctr[1] = lo1;
        ctr[2] = hi0 ^ ctr[3] ^ key[1];
        ctr[3] = lo0;
    }

    void generate_block() {
        std::uint32_t ctr[4] = {counter_[0], counter_[1], counter_[2], counter_[3]};
        std::uint32_t key[2] = {key_[0], key_[1]};
        for (int round = 0; round < 10; ++round) {
            philox_round(ctr, key);
            if (round < 9) {
                key[0] += kPhiloxW0;
                key[1] += kPhiloxW1;
            }
        }
        block_[0] = ctr[0];
        block_[1] = ctr[1];
        block_[2] = ctr[2];
        block_[3] = ctr[3];
        ++counter_[0];
        if (counter_[0] == 0) {
            ++counter_[1];
        }
        lane_ = 4;
    }

    std::uint32_t counter_[4]{};
    std::uint32_t key_[2]{};
    std::uint32_t block_[4]{};
    int lane_{4};
};

}  // namespace distributions::detail::simd

#endif
