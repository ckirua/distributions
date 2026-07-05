#pragma once

#include "distributions/detail/multivariate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Dirichlet {
    double a0_;
    double a1_;
    double a2_;
    Dirichlet(double a0, double a1, double a2) : a0_(a0), a1_(a1), a2_(a2) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_dirichlet_first(rng, a0_, a1_, a2_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
