#pragma once

#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Shifted {
    double loc_;
    double rate_;
    Shifted(double loc, double rate) : loc_(loc), rate_(rate) {
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(rate_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_shifted_exponential(rng, loc_, rate_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
