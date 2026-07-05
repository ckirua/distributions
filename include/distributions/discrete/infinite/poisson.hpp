#pragma once

#include "distributions/detail/poisson.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Poisson {
    double mu_;
    Poisson(double mu) : mu_(mu) {
        detail::assert_nonnegative(mu_);
    }

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_poisson(rng, mu_);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
