#pragma once

#include <cstddef>

namespace distributions {

template <typename Derived, typename SampleT, typename RNG>
struct DistributionBase {
    void sample_batch(SampleT* out, std::size_t n, RNG& rng) {
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = static_cast<Derived*>(this)->sample(rng);
        }
    }
};

}  // namespace distributions
