#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/uniform.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct BatesDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    int n_;
    double lo_;
    double hi_;
    BatesDistribution(int n, double lo, double hi) : n_(n), lo_(lo), hi_(hi) {
        detail::assert_nonnegative_int(n_);
        detail::assert_double_interval(lo_, hi_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        double sum = 0.0;
                for (int i = 0; i < n_; ++i) { sum += detail::sample_uniform(rng, lo_, hi_); }
                return static_cast<Sample>(sum / static_cast<double>(n_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using Bates = BatesDistribution<double>;

}  // namespace distributions
