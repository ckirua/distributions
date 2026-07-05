#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/semi_infinite.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct Benktander1stKindDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double a_;
    double b_;
    Benktander1stKindDistribution(double a, double b) : a_(a), b_(b) {
        detail::assert_strictly_positive(a_);
        detail::assert_strictly_positive(b_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_benktander(rng, a_, b_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Benktander1stKind = Benktander1stKindDistribution<double>;

}  // namespace distributions
