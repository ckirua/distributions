#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/detail/fast/inverse_gamma.hpp"
#include "distributions/detail/gamma.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct InverseGammaDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double shape_;
    double scale_;
    InverseGammaDistribution(double shape, double scale) : shape_(shape), scale_(scale) {
        detail::assert_strictly_positive(shape_);
        detail::assert_strictly_positive(scale_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(scale_ / detail::sample_gamma(rng, shape_, 1.0));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        if constexpr (std::is_same_v<Sample, double>) {
            if (n >= detail::kFastThreshold) {
                detail::fast::inverse_gamma_sample_batch(out, n, shape_, scale_, detail::batch_seed_from(rng));
                return;
            }
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using InverseGamma = InverseGammaDistribution<double>;

}  // namespace distributions
