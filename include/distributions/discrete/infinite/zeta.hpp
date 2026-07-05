#pragma once

#include "distributions/detail/validate.hpp"
#include "distributions/discrete/finite/zipf.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Zeta {
    int N_;
    double s_;
    Zeta(int N, double s) : N_(N), s_(s) {
        detail::assert_positive_int(N_);
        detail::assert_strictly_positive(s_);
    }

    [[nodiscard]] int sample(Pcg32& rng) const {
        return Zipf(N_, s_).sample(rng);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
