#pragma once

#include "distributions/detail/poisson.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Panjer {
    double lambda_;
    Panjer(double lambda) : lambda_(lambda) {
        detail::assert_nonnegative(lambda_);
    }

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_poisson(rng, lambda_);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
