#include "distributions/concepts.hpp"
#include "distributions/continuous/real_line/normal.hpp"
#include "distributions/continuous/semi_infinite/exponential.hpp"
#include "distributions/discrete/finite/bernoulli.hpp"
#include "distributions/discrete/finite/beta_binomial.hpp"
#include "distributions/discrete/finite/binomial.hpp"
#include "distributions/discrete/finite/categorical.hpp"
#include "distributions/discrete/finite/discrete_uniform.hpp"
#include "distributions/discrete/finite/poisson_binomial.hpp"
#include "distributions/discrete/finite/zipf.hpp"
#include "distributions/discrete/infinite/geometric.hpp"
#include "distributions/discrete/infinite/negative_binomial.hpp"
#include "distributions/discrete/infinite/skellam.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <type_traits>

namespace {

template <typename Dist, typename Sample>
constexpr bool models_distribution() {
    return distributions::Distribution<Dist, Sample, distributions::Pcg32>;
}

template <typename Dist, typename Sample>
bool runtime_models_distribution(const Dist& dist) {
    (void)dist;
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
    static_assert(models_distribution<distributions::BernoulliDistribution<int>, int>());
    static_assert(models_distribution<distributions::BernoulliDistribution<std::int32_t>, std::int32_t>());
    static_assert(models_distribution<distributions::DiscreteUniform, int>());
    static_assert(models_distribution<distributions::DiscreteUniformDistribution<int>, int>());
    static_assert(models_distribution<distributions::DiscreteUniformDistribution<std::int32_t>, std::int32_t>());
    static_assert(models_distribution<distributions::Binomial, int>());
    static_assert(models_distribution<distributions::Categorical, int>());
    static_assert(models_distribution<distributions::BetaBinomial, int>());
    static_assert(models_distribution<distributions::PoissonBinomial, int>());
    static_assert(models_distribution<distributions::Zipf, int>());
    static_assert(models_distribution<distributions::ZipfMandelbrot, int>());
    static_assert(models_distribution<distributions::Geometric, int>());
    static_assert(models_distribution<distributions::NegativeBinomial, int>());
    static_assert(models_distribution<distributions::Skellam, int>());
    static_assert(models_distribution<distributions::Exponential, double>());
    static_assert(models_distribution<distributions::Normal, double>());
    static_assert(models_distribution<distributions::ExponentialDistribution<float>, float>());
    static_assert(models_distribution<distributions::ExponentialDistribution<double>, double>());
    static_assert(models_distribution<distributions::NormalDistribution<float>, float>());
    static_assert(models_distribution<distributions::NormalDistribution<double>, double>());

    static_assert(std::is_same_v<distributions::Bernoulli, distributions::BernoulliDistribution<int>>);
    static_assert(
        std::is_same_v<distributions::DiscreteUniform, distributions::DiscreteUniformDistribution<int>>);
    static_assert(std::is_same_v<distributions::Normal, distributions::NormalDistribution<double>>);
    static_assert(
        std::is_same_v<distributions::Exponential, distributions::ExponentialDistribution<double>>);
    static_assert(std::is_same_v<distributions::sample_type_t<distributions::Bernoulli>, int>);
    static_assert(std::is_same_v<distributions::sample_type_t<distributions::Normal>, double>);
    static_assert(std::is_same_v<distributions::sample_type_t<distributions::Exponential>, double>);

    const distributions::Bernoulli bern(0.5);
    const distributions::DiscreteUniform du(1, 7);
    const distributions::Binomial binom(20, 0.4);
    const distributions::Categorical cat({0.5, 0.3, 0.2});
    const distributions::BetaBinomial bb(20, 2.0, 5.0);
    const distributions::PoissonBinomial pb({0.1, 0.2, 0.3, 0.4});
    const distributions::Zipf zipf(10, 2.0);
    const distributions::ZipfMandelbrot zm(10, 1.0, 2.0);
    const distributions::Geometric geom(0.25);
    const distributions::NegativeBinomial nb(5, 0.4);
    const distributions::Skellam skellam(3.0, 1.0);
    const distributions::Exponential exp(1.5);
    const distributions::Normal normal(0.0, 1.0);

    if (!runtime_models_distribution<distributions::Bernoulli, int>(bern)) {
        std::cerr << "Bernoulli failed runtime concept check\n";
        return EXIT_FAILURE;
    }
    if (!runtime_models_distribution<distributions::DiscreteUniform, int>(du)) {
        std::cerr << "DiscreteUniform failed runtime concept check\n";
        return EXIT_FAILURE;
    }
    if (!runtime_models_distribution<distributions::Binomial, int>(binom)) {
        std::cerr << "Binomial failed runtime concept check\n";
        return EXIT_FAILURE;
    }
    if (!runtime_models_distribution<distributions::Categorical, int>(cat)) {
        std::cerr << "Categorical failed runtime concept check\n";
        return EXIT_FAILURE;
    }
    if (!runtime_models_distribution<distributions::BetaBinomial, int>(bb)) {
        std::cerr << "BetaBinomial failed runtime concept check\n";
        return EXIT_FAILURE;
    }
    if (!runtime_models_distribution<distributions::PoissonBinomial, int>(pb)) {
        std::cerr << "PoissonBinomial failed runtime concept check\n";
        return EXIT_FAILURE;
    }
    if (!runtime_models_distribution<distributions::Zipf, int>(zipf)) {
        std::cerr << "Zipf failed runtime concept check\n";
        return EXIT_FAILURE;
    }
    if (!runtime_models_distribution<distributions::ZipfMandelbrot, int>(zm)) {
        std::cerr << "ZipfMandelbrot failed runtime concept check\n";
        return EXIT_FAILURE;
    }
    if (!runtime_models_distribution<distributions::Geometric, int>(geom)) {
        std::cerr << "Geometric failed runtime concept check\n";
        return EXIT_FAILURE;
    }
    if (!runtime_models_distribution<distributions::NegativeBinomial, int>(nb)) {
        std::cerr << "NegativeBinomial failed runtime concept check\n";
        return EXIT_FAILURE;
    }
    if (!runtime_models_distribution<distributions::Skellam, int>(skellam)) {
        std::cerr << "Skellam failed runtime concept check\n";
        return EXIT_FAILURE;
    }
    if (!runtime_models_distribution<distributions::Exponential, double>(exp)) {
        std::cerr << "Exponential failed runtime concept check\n";
        return EXIT_FAILURE;
    }
    if (!runtime_models_distribution<distributions::Normal, double>(normal)) {
        std::cerr << "Normal failed runtime concept check\n";
        return EXIT_FAILURE;
    }

    const distributions::BernoulliDistribution<std::int32_t> bern_i32(0.5);
    if (!runtime_models_distribution<distributions::BernoulliDistribution<std::int32_t>, std::int32_t>(
            bern_i32)) {
        std::cerr << "BernoulliDistribution<int32_t> failed runtime concept check\n";
        return EXIT_FAILURE;
    }

    const distributions::NormalDistribution<float> normal_f(1.0, 0.5);
    const distributions::ExponentialDistribution<float> exp_f(1.5);
    distributions::Pcg32 float_rng(12345);
    float normal_samples[4096];
    float exp_samples[4096];
    normal_f.sample_batch(normal_samples, 4096, float_rng);
    exp_f.sample_batch(exp_samples, 4096, float_rng);
    double normal_sum = 0.0;
    double exp_sum = 0.0;
    for (std::size_t i = 0; i < 4096; ++i) {
        if (!std::isfinite(normal_samples[i]) || !std::isfinite(exp_samples[i])) {
            std::cerr << "non-finite float sample\n";
            return EXIT_FAILURE;
        }
        normal_sum += static_cast<double>(normal_samples[i]);
        exp_sum += static_cast<double>(exp_samples[i]);
    }
    if (std::abs(normal_sum / 4096.0 - normal_f.mean()) > 0.08) {
        std::cerr << "NormalDistribution<float> mean drift\n";
        return EXIT_FAILURE;
    }
    if (std::abs(exp_sum / 4096.0 - exp_f.mean()) > 0.08) {
        std::cerr << "ExponentialDistribution<float> mean drift\n";
        return EXIT_FAILURE;
    }

    static_assert(sizeof(int) == sizeof(std::int32_t));
    const distributions::BernoulliDistribution<std::int32_t> bern_i32_fast(0.35);
    std::int32_t bern_i32_out[5000];
    distributions::Pcg32 i32_rng(777);
    bern_i32_fast.sample_batch(bern_i32_out, 5000, i32_rng);
    double bern_i32_mean = 0.0;
    for (std::size_t i = 0; i < 5000; ++i) {
        bern_i32_mean += static_cast<double>(bern_i32_out[i]);
    }
    if (std::abs(bern_i32_mean / 5000.0 - bern_i32_fast.mean()) > 0.05) {
        std::cerr << "BernoulliDistribution<int32_t> Tier B mean drift\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
