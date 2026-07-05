#include "distributions/all.hpp"

#include "distributions/rng.hpp"

#include <cstdlib>
#include <iostream>

int main() {
    distributions::Pcg32 rng(42);

    distributions::Bernoulli bern(0.35);
    distributions::DiscreteUniform dunif(1, 7);
    distributions::Binomial binom(20, 0.4);
    distributions::Categorical cat({0.1, 0.2, 0.3, 0.4});
    distributions::BetaBinomial bb(20, 2.0, 5.0);
    distributions::PoissonBinomial pb({0.1, 0.2, 0.15, 0.25});
    distributions::Zipf zipf(100, 2.0);
    distributions::ZipfMandelbrot zm(100, 1.0, 2.0);
    distributions::Geometric geom(0.3);
    distributions::NegativeBinomial nb(5, 0.4);
    distributions::Skellam sk(1.0, 0.8);
    distributions::Exponential exp(1.5);
    distributions::Normal normal(0.0, 1.0);
    distributions::GeneralizedHyperbolic gh(0.0, 1.5, 0.5);

    int i_out = 0;
    double d_out = 0.0;
    (void)bern.sample(rng);
    (void)dunif.sample(rng);
    (void)binom.sample(rng);
    (void)cat.sample(rng);
    (void)bb.sample(rng);
    (void)pb.sample(rng);
    (void)zipf.sample(rng);
    (void)zm.sample(rng);
    (void)geom.sample(rng);
    (void)nb.sample(rng);
    (void)sk.sample(rng);
    (void)exp.sample(rng);
    (void)normal.sample(rng);
    (void)gh.sample(rng);

    bern.sample_batch(&i_out, 1, rng);
    exp.sample_batch(&d_out, 1, rng);
    gh.sample_batch(&d_out, 1, rng);

    std::cout << "param validation smoke ok\n";
    return EXIT_SUCCESS;
}
