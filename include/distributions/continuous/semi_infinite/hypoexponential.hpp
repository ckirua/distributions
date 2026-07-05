#pragma once

#include "distributions/detail/semi_infinite.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Hypoexponential {
    double rate1_;
    double rate2_;
    Hypoexponential(double rate1, double rate2) : rate1_(rate1), rate2_(rate2) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_hypoexponential(rng, rate1_, rate2_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
