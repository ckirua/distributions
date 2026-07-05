#pragma once

#include "distributions/detail/real_line.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct NoncentralT {
    double df_;
    double nc_;
    double loc_;
    double scale_;
    NoncentralT(double df, double nc, double loc, double scale) : df_(df), nc_(nc), loc_(loc), scale_(scale) {}

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_nct(rng, df_, nc_, loc_, scale_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
