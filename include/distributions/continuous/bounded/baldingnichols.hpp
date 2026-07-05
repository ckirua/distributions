#pragma once

#include "distributions/detail/bounded.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Baldingnichols {
    double f_;
    Baldingnichols(double f) : f_(f) {
        detail::assert_strictly_positive(f_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_balding_nichols(rng, f_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
