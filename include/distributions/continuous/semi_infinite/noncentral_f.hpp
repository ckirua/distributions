#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct NoncentralFDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double dfn_;
    double dfd_;
    double nc_;
    NoncentralFDistribution(double dfn, double dfd, double nc) : dfn_(dfn), dfd_(dfd), nc_(nc) {
        detail::assert_strictly_positive(dfn_);
        detail::assert_strictly_positive(dfd_);
        detail::assert_nonnegative(nc_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_noncentral_f(rng, dfn_, dfd_, nc_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using NoncentralF = NoncentralFDistribution<double>;

}  // namespace distributions
