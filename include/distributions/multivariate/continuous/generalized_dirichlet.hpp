#pragma once

#include "distributions/detail/multivariate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct GeneralizedDirichlet {
    double a1_;
    double b1_;
    double a2_;
    double b2_;
    GeneralizedDirichlet(double a1, double b1, double a2, double b2) : a1_(a1), b1_(b1), a2_(a2), b2_(b2) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_generalized_dirichlet_first(rng, a1_, b1_, a2_, b2_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
