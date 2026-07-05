#pragma once

#include "distributions/detail/real_line.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct GeneralizedLogisticLogisticBeta {
    double c_;
    double loc_;
    double scale_;
    GeneralizedLogisticLogisticBeta(double c, double loc, double scale) : c_(c), loc_(loc), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_genlogistic(rng, c_, loc_, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
