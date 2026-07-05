#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/f_distribution.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct FDistributionDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double dfn_;
    double dfd_;
    FDistributionDistribution(double dfn, double dfd) : dfn_(dfn), dfd_(dfd) {
        detail::assert_strictly_positive(dfn_);
        detail::assert_strictly_positive(dfd_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_f(rng, dfn_, dfd_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using FDistribution = FDistributionDistribution<double>;

}  // namespace distributions
