#pragma once

#include "distributions/detail/special.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Soliton {
    int n_max_;
    Soliton(int n_max) : n_max_(n_max) {
        detail::assert_positive_int(n_max_);
    }

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_soliton(rng, n_max_);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
