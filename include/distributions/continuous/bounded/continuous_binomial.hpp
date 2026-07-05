#pragma once

#include "distributions/detail/bounded.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct ContinuousBinomial {
    int n_;
    ContinuousBinomial(int n) : n_(n) {
        detail::assert_nonnegative_int(n_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_continuous_binomial(rng, n_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
