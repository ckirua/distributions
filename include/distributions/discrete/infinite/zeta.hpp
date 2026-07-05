#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/discrete/finite/zipf.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = int>
struct ZetaDistribution {
    static_assert(is_discrete_sample_v<Sample>);

    int N_;
    double s_;
    ZetaDistribution(int N, double s) : N_(N), s_(s) {
        detail::assert_positive_int(N_);
        detail::assert_strictly_positive(s_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(Zipf(N_, s_).sample(rng));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Zeta = ZetaDistribution<int>;

}  // namespace distributions
