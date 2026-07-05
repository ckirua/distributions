#pragma once

#include "distributions/rng.hpp"

#include <cassert>
#include <cstddef>
#include <span>

namespace cydist_shim {

template<typename T>
[[nodiscard]] inline std::span<T> checked_output_span(T* data, std::size_t size) {
    assert((data != nullptr) || (size == 0));
    return std::span<T>(data, size);
}

template<typename Dist, typename T>
inline void sample_batch(Dist& dist, T* out, std::size_t n_samples, distributions::Pcg32& rng) {
    const std::span<T> out_span = checked_output_span(out, n_samples);
    dist.sample_batch(out_span.data(), out_span.size(), rng);
}

}  // namespace cydist_shim
