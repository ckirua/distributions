#pragma once

#include "distributions/detail/semi_infinite.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct PhaseType {
    double rate1_;
    double rate2_;
    double rate3_;
    PhaseType(double rate1, double rate2, double rate3) : rate1_(rate1), rate2_(rate2), rate3_(rate3) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_phase_type(rng, rate1_, rate2_, rate3_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
