#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/poisson.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct PanjerDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    double lambda_;
    PanjerDistribution(double lambda) : lambda_(lambda) {
        detail::assert_nonnegative(lambda_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_poisson(rng, lambda_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Panjer = PanjerDistribution<int>;

}  // namespace distributions
