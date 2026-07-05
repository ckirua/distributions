#pragma once

#include "distributions/detail/counter_rng.hpp"
#include "distributions/detail/fast/categorical.hpp"
#include "distributions/detail/fast/common.hpp"
#include "distributions/rng.hpp"

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace distributions {

struct Categorical {
    explicit Categorical(std::vector<double> probs) { set_probs(std::move(probs)); }

    [[nodiscard]] int sample(Pcg32& rng) const {
        const int k = static_cast<int>(probs_.size());
        const int col = static_cast<int>(rng.next_u32() % static_cast<std::uint32_t>(k));
        const double u = rng.next_double();
        return u < prob_[static_cast<std::size_t>(col)]
                   ? col
                   : alias_[static_cast<std::size_t>(col)];
    }

    void sample_batch(int* out, std::size_t n, Pcg32& rng) const {
        if (n >= detail::kFastThreshold) {
            detail::fast::categorical_sample_batch(
                out,
                n,
                static_cast<int>(prob_.size()),
                alias_.data(),
                prob_.data(),
                detail::batch_seed_from(rng));
            return;
        }
        for (std::size_t i = 0; i < n; ++i) {
            out[i] = sample(rng);
        }
    }

    [[nodiscard]] double mean() const { return 0.0; }

    [[nodiscard]] double variance() const { return 0.0; }

private:
    std::vector<double> probs_;
    std::vector<int> alias_;
    std::vector<double> prob_;

    void set_probs(std::vector<double> probs) {
        if (probs.empty()) {
            throw std::invalid_argument("categorical requires at least one category");
        }
        double sum = 0.0;
        for (double p : probs) {
            sum += p;
        }
        const int k = static_cast<int>(probs.size());
        probs_.assign(static_cast<std::size_t>(k), 0.0);
        for (int i = 0; i < k; ++i) {
            probs_[static_cast<std::size_t>(i)] = probs[static_cast<std::size_t>(i)] / sum;
        }
        alias_.assign(static_cast<std::size_t>(k), 0);
        prob_.assign(static_cast<std::size_t>(k), 0.0);
        std::vector<int> small;
        std::vector<int> large;
        for (int i = 0; i < k; ++i) {
            prob_[static_cast<std::size_t>(i)] = probs_[static_cast<std::size_t>(i)] * k;
            if (prob_[static_cast<std::size_t>(i)] < 1.0) {
                small.push_back(i);
            } else {
                large.push_back(i);
            }
        }
        while (!small.empty() && !large.empty()) {
            const int s = small.back();
            small.pop_back();
            const int l = large.back();
            large.pop_back();
            alias_[static_cast<std::size_t>(s)] = l;
            prob_[static_cast<std::size_t>(l)] =
                prob_[static_cast<std::size_t>(l)] + prob_[static_cast<std::size_t>(s)] - 1.0;
            if (prob_[static_cast<std::size_t>(l)] < 1.0) {
                small.push_back(l);
            } else {
                large.push_back(l);
            }
        }
    }
};

}  // namespace distributions
