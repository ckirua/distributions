#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"
#include <cmath>
#include <cstddef>

namespace distributions {

struct Pareto {
    double alpha_;
    double scale_;
    Pareto(double alpha, double scale) : alpha_(alpha), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return scale_ / std::pow(rng.next_double(), 1.0 / alpha_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
