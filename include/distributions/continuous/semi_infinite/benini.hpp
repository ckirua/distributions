#pragma once

#include "distributions/detail/semi_infinite.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Benini {
    double alpha_;
    double beta_;
    double gamma_;
    Benini(double alpha, double beta, double gamma) : alpha_(alpha), beta_(beta), gamma_(gamma) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_benini(rng, alpha_, beta_, gamma_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
