#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct Gumbel {
    double loc_;
    double scale_;
    Gumbel(double loc, double scale) : loc_(loc), scale_(scale) {
        detail::assert_finite(loc_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return loc_ - scale_ * std::log(-std::log(rng.next_double()));
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
