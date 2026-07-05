#pragma once

#include "distributions/detail/discrete.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Conwaymaxwellpoisson {
    double lambda_;
    double nu_;
    Conwaymaxwellpoisson(double lambda, double nu) : lambda_(lambda), nu_(nu) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_comp_poisson(rng, lambda_, nu_);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
