#pragma once

#include "distributions/detail/matrix.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct Lkj {
    double eta_;
    Lkj(double eta) : eta_(eta) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_lkj_rho(rng, eta_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
