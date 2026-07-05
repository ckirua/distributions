#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Uniform {
    double lo_;
    double hi_;
    Uniform(double lo, double hi) : lo_(lo), hi_(hi) {
        detail::assert_double_interval(lo_, hi_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_uniform(rng, lo_, hi_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
