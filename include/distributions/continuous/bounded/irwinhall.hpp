#pragma once

#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Irwinhall {
    int n_;
    double lo_;
    double hi_;
    Irwinhall(int n, double lo, double hi) : n_(n), lo_(lo), hi_(hi) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        double sum = 0.0;
        for (int i = 0; i < n_; ++i) { sum += detail::sample_uniform(rng, lo_, hi_); }
        return sum / static_cast<double>(n_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
