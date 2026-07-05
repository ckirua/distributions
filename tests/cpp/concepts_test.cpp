#include "distributions/concepts.hpp"
#include "distributions/continuous/real_line/normal.hpp"
#include "distributions/discrete/finite/bernoulli.hpp"
#include "distributions/discrete/finite/discrete_uniform.hpp"

#include <cstdlib>
#include <iostream>
#include <type_traits>

namespace {

template <typename Dist, typename Sample>
constexpr bool models_distribution() {
    return distributions::Distribution<Dist, Sample, distributions::Pcg32>;
}

}  // namespace

int main() {
    static_assert(distributions::is_discrete_sample_v<int>);
    static_assert(distributions::is_discrete_sample_v<std::int32_t>);
    static_assert(distributions::is_continuous_sample_v<float>);
    static_assert(distributions::is_continuous_sample_v<double>);
    static_assert(!distributions::is_continuous_sample_v<int>);

    static_assert(models_distribution<distributions::Bernoulli, int>());
    static_assert(models_distribution<distributions::DiscreteUniform, int>());
    static_assert(models_distribution<distributions::Normal, double>());

    static_assert(std::is_same_v<distributions::sample_type_t<distributions::Bernoulli>, int>);
    static_assert(std::is_same_v<distributions::sample_type_t<distributions::Normal>, double>);

    distributions::Bernoulli bern(0.5);
    distributions::Normal normal(0.0, 1.0);
    if (!distributions::Distribution<decltype(bern), int>) {
        std::cerr << "Bernoulli failed runtime concept check\n";
        return EXIT_FAILURE;
    }
    if (!distributions::Distribution<decltype(normal), double>) {
        std::cerr << "Normal failed runtime concept check\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
