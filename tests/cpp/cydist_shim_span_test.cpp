#include "cydist_shim.h"

#include <cstdlib>
#include <vector>

int main() {
    std::vector<double> out(128);
    cydist_exponential_sample_batch(1.0, 42, out.data(), out.size());
    for (const double x : out) {
        if (x < 0.0) {
            return EXIT_FAILURE;
        }
    }

    cydist_exponential_sample_batch(1.0, 42, nullptr, 0);

    std::vector<int> out_i(64);
    cydist_poisson_sample_batch(2.5, 7, out_i.data(), out_i.size());
    for (const int k : out_i) {
        if (k < 0) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
