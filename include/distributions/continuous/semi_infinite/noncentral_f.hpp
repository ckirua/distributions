#pragma once

#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <cstddef>

namespace distributions {

struct NoncentralF {
    double dfn_;
    double dfd_;
    double nc_;
    NoncentralF(double dfn, double dfd, double nc) : dfn_(dfn), dfd_(dfd), nc_(nc) {
        detail::assert_strictly_positive(dfn_);
        detail::assert_strictly_positive(dfd_);
        detail::assert_nonnegative(nc_);
    }

    [[nodiscard]] double sample(Pcg32& rng) const {
        return detail::sample_noncentral_f(rng, dfn_, dfd_, nc_);
    }

    void sample_batch(double* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

}  // namespace distributions
