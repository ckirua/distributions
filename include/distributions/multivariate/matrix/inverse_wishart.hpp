#pragma once

#include "distributions/detail/matrix.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct InverseWishart {
    double df_;
    double v00_;
    double v01_;
    double v11_;
    InverseWishart(double df, double v00, double v01, double v11) : df_(df), v00_(v00), v01_(v01), v11_(v11) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_invwishart_trace(rng, df_, v00_, v01_, v11_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
