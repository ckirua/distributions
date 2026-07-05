#include "pcg.ispc"

export void dummy_sample_batch(uniform int out[], uniform int n, uniform unsigned int64 seed) {
    foreach (i = 0 ... n) {
        out[i] = 0;
    }
    if (seed == 0) {
        out[0] = 0;
    }
}

export void bernoulli_sample_batch(
    uniform int out[], uniform int n, uniform double p, uniform unsigned int64 seed) {
    foreach (i = 0 ... n) {
        const double u = splitmix_double(seed, i);
        out[i] = u < p ? 1 : 0;
    }
}

export void discrete_uniform_sample_batch(
    uniform int out[], uniform int n, uniform int low, uniform int high,
    uniform unsigned int64 seed) {
    const uniform int span = high - low + 1;
    foreach (i = 0 ... n) {
        const int u = (int)(splitmix_double(seed, i) * (double)span);
        out[i] = low + u;
    }
}

export void binomial_sample_batch(
    uniform int out[], uniform int n, uniform int trials, uniform double p,
    uniform unsigned int64 seed) {
    foreach (i = 0 ... n) {
        uniform double cumulative = 0.0;
        uniform double prob = pow(1.0 - p, (uniform double)trials);
        const double u = splitmix_double(seed, i);
        cumulative = prob;
        int result = 0;
        if (u <= cumulative) {
            result = 0;
        } else {
            for (uniform int k = 1; k <= trials; ++k) {
                prob *= ((double)(trials - k + 1) / (double)k) * (p / (1.0 - p));
                cumulative += prob;
                if (u <= cumulative) {
                    result = k;
                    break;
                }
            }
        }
        out[i] = result;
    }
}

export void geometric_sample_batch(
    uniform int out[], uniform int n, uniform double p, uniform unsigned int64 seed) {
    foreach (i = 0 ... n) {
        const double u = splitmix_double(seed, i);
        out[i] = (int)floor(log(1.0 - u) / log(1.0 - p)) + 1;
    }
}

export void exponential_sample_batch(
    uniform double out[], uniform int n, uniform double rate, uniform unsigned int64 seed) {
    foreach (i = 0 ... n) {
        const double u = splitmix_double(seed, i);
        out[i] = -log(1.0 - u) / rate;
    }
}

export void normal_sample_batch(
    uniform double out[], uniform int n, uniform double mu, uniform double sigma,
    uniform unsigned int64 seed) {
    foreach (i = 0 ... n) {
        const double u1 = max(splitmix_double(seed, 2 * i), (double)1e-300);
        const double u2 = splitmix_double(seed, 2 * i + 1);
        const double z = sqrt(-2.0 * log(u1)) * cos(6.283185307179586 * u2);
        out[i] = mu + sigma * z;
    }
}

export void skellam_sample_batch(
    uniform int out[], uniform int n, uniform double mu1, uniform double mu2,
    uniform unsigned int64 seed) {
    foreach (i = 0 ... n) {
        const uniform double limit1 = exp(-mu1);
        varying double p1 = 1.0;
        varying int k1 = 0;
        do {
            ++k1;
            p1 *= splitmix_double(seed, 3 * i + k1);
        } while (p1 > limit1);
        const uniform double limit2 = exp(-mu2);
        varying double p2 = 1.0;
        varying int k2 = 0;
        do {
            ++k2;
            p2 *= splitmix_double(seed, 3 * i + 100 + k2);
        } while (p2 > limit2);
        out[i] = (k1 - 1) - (k2 - 1);
    }
}

export void poisson_binomial_sample_batch(
    uniform int out[], uniform int n, uniform int num_trials, uniform double p_common,
    uniform unsigned int64 seed) {
    foreach (i = 0 ... n) {
        int sum = 0;
        for (uniform int t = 0; t < num_trials; ++t) {
            if (splitmix_double(seed, i * num_trials + t) < p_common) {
                ++sum;
            }
        }
        out[i] = sum;
    }
}

export void beta_binomial_sample_batch(
    uniform int out[], uniform int n, uniform int trials, uniform double alpha,
    uniform double beta, uniform unsigned int64 seed) {
    foreach (i = 0 ... n) {
        const uniform double p = alpha / (alpha + beta);
        int sum = 0;
        for (uniform int t = 0; t < trials; ++t) {
            if (splitmix_double(seed, i * trials + t) < p) {
                ++sum;
            }
        }
        out[i] = sum;
    }
}

export void negative_binomial_sample_batch(
    uniform int out[], uniform int n, uniform int r, uniform double p,
    uniform unsigned int64 seed) {
    foreach (i = 0 ... n) {
        int failures = 0;
        int successes = 0;
        while (successes < r) {
            if (splitmix_double(seed, i * (r + failures + 1) + failures) < p) {
                ++successes;
            } else {
                ++failures;
            }
        }
        out[i] = failures;
    }
}

export void zipf_sample_batch(
    uniform int out[], uniform int n, uniform int N, uniform double s,
    uniform unsigned int64 seed) {
    foreach (i = 0 ... n) {
        uniform double sum = 0.0;
        for (uniform int k = 1; k <= N; ++k) {
            sum += 1.0 / pow((uniform double)k, s);
        }
        const double u = splitmix_double(seed, i) * sum;
        uniform double acc = 0.0;
        int result = N;
        for (uniform int k = 1; k <= N; ++k) {
            acc += 1.0 / pow((uniform double)k, s);
            if (u <= acc) {
                result = k;
                break;
            }
        }
        out[i] = result;
    }
}

export void zipf_mandelbrot_sample_batch(
    uniform int out[], uniform int n, uniform int N, uniform double q, uniform double s,
    uniform unsigned int64 seed) {
    foreach (i = 0 ... n) {
        uniform double sum = 0.0;
        for (uniform int k = 1; k <= N; ++k) {
            sum += 1.0 / pow((uniform double)k + q, s);
        }
        const double u = splitmix_double(seed, i) * sum;
        uniform double acc = 0.0;
        int result = N;
        for (uniform int k = 1; k <= N; ++k) {
            acc += 1.0 / pow((uniform double)k + q, s);
            if (u <= acc) {
                result = k;
                break;
            }
        }
        out[i] = result;
    }
}

export void categorical_sample_batch(
    uniform int out[], uniform int n, uniform int k, uniform double probs[],
    uniform unsigned int64 seed) {
    foreach (i = 0 ... n) {
        const double u = splitmix_double(seed, i);
        uniform double acc = 0.0;
        int result = k - 1;
        for (uniform int j = 0; j < k; ++j) {
            acc += probs[j];
            if (u <= acc) {
                result = j;
                break;
            }
        }
        out[i] = result;
    }
}
