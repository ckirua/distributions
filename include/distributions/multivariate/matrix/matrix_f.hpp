#pragma once

#include <cstddef>
#include "distributions/concepts.hpp"
#include "distributions/detail/matrix.hpp"
#include "distributions/detail/validate.hpp"
#include "distributions/rng.hpp"
#include <type_traits>

namespace distributions {

template <typename Sample = double>
struct MatrixFDistribution {
    static_assert(is_continuous_sample_v<Sample>);

    double df1_;
    double df2_;
    double v00_;
    double v11_;
    MatrixFDistribution(double df1, double df2, double v00, double v11) : df1_(df1), df2_(df2), v00_(v00), v11_(v11) {
        detail::assert_finite(df1_);
        detail::assert_finite(df2_);
        detail::assert_finite(v00_);
        detail::assert_finite(v11_);
    }

    [[nodiscard]] Sample sample(Pcg32& rng) const {
        return static_cast<Sample>(detail::sample_matrix_f_trace(rng, df1_, df2_, v00_, v11_));
    }

    void sample_batch(Sample* out, std::size_t n, Pcg32& rng) const {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }
};

using MatrixF = MatrixFDistribution<double>;

}  // namespace distributions
