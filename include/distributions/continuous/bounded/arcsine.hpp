#pragma once

#include "distributions/detail/gamma.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Arcsine {

    Arcsine() = default;

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_beta(rng, 0.5, 0.5);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
