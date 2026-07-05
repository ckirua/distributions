/* Example: C ABI via cydist_shim.h (no C++ headers required). */
/* Build: make build  →  ./build/example_c_api */

#include "cydist_shim.h"

#include <stdio.h>

int main(void) {
    enum { n = 8 };
    int poisson_out[n];
    double normal_out[n];

    cydist_poisson_sample_batch(4.0, 42, poisson_out, (size_t)n);
    cydist_normal_sample_batch(0.0, 1.0, 42, normal_out, (size_t)n);

    printf("poisson (mu=4): ");
    for (int i = 0; i < n; ++i) {
        printf("%d ", poisson_out[i]);
    }
    printf("\n");

    printf("normal (0,1):   ");
    for (int i = 0; i < n; ++i) {
        printf("%.3f ", normal_out[i]);
    }
    printf("\n");

    return 0;
}
