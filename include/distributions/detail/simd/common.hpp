#pragma once

#include "distributions/detail/fast/splitmix_stream.hpp"

#include <cstdint>

namespace distributions::detail::simd {

using SplitMix64Stream = fast::SplitMix64Stream;

inline std::uint64_t lane_seed(std::uint64_t seed, std::uint32_t lane) {
    return seed ^ (static_cast<std::uint64_t>(lane) * 0x9e3779b97f4a7c15ULL);
}

}  // namespace distributions::detail::simd
