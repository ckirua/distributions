#pragma once

#include "distributions/detail/discrete.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct DiscretePhaseType {
    double p1_;
    double p2_;
    DiscretePhaseType(double p1, double p2) : p1_(p1), p2_(p2) {}

    [[nodiscard]] int sample(Pcg32& rng) const {
        return detail::sample_discrete_phase_type(rng, p1_, p2_);
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
