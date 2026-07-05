#pragma once

#include "distributions/detail/f_distribution.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct FDistribution {
    double dfn_;
    double dfd_;
    FDistribution(double dfn, double dfd) : dfn_(dfn), dfd_(dfd) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_f(rng, dfn_, dfd_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
