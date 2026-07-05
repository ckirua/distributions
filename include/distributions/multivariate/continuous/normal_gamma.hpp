#pragma once

#include "distributions/detail/multivariate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct NormalGamma {
    double shape_;
    double rate_;
    NormalGamma(double shape, double rate) : shape_(shape), rate_(rate) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_normal_gamma_mean(rng, shape_, rate_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
