#pragma once

#include "distributions/detail/discrete.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Borel {
    double mu_;
    Borel(double mu) : mu_(mu) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_borel(rng, mu_);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
