#pragma once

#include "distributions/detail/bounded.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Argus {
    double chi_;
    Argus(double chi) : chi_(chi) {
        detail::assert_strictly_positive(chi_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_argus(rng, chi_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
