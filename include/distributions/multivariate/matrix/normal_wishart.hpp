#pragma once

#include "distributions/detail/matrix.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct NormalWishart {
    double df_;
    double kappa_;
    double v00_;
    double v11_;
    NormalWishart(double df, double kappa, double v00, double v11) : df_(df), kappa_(kappa), v00_(v00), v11_(v11) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_normal_wishart_mean(rng, df_, kappa_, v00_, v11_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
