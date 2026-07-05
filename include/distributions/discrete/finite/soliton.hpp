#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/special.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct SolitonDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    int n_max_;
    SolitonDistribution(int n_max) : n_max_(n_max) {
        detail::assert_positive_int(n_max_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_soliton(rng, n_max_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Soliton = SolitonDistribution<int>;

}  // namespace distributions
