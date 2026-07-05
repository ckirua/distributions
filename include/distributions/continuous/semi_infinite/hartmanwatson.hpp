#pragma once

#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Hartmanwatson {
    double nu_;
    Hartmanwatson(double nu) : nu_(nu) {
        detail::assert_strictly_positive(nu_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_hartman_watson(rng, nu_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
