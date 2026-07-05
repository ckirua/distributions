#pragma once

#include "distributions/detail/bounded.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Pert {
    double lo_;
    double mode_;
    double hi_;
    Pert(double lo, double mode, double hi) : lo_(lo), mode_(mode), hi_(hi) {
        detail::assert_finite(mode_);
        detail::assert_double_interval(lo_, hi_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_pert(rng, lo_, mode_, hi_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
