#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

static size_t estimate_sieve_size(size_t n)
{
    /* Small constant for small n */
    if (n < 6) {
        return 12;
    }

    /* Estimated upper bound for nth prime */
    double ln = log(n);
    double estimate = n * (ln + log(ln));
    return (size_t)ceil(estimate);
}

static size_t sieve_of_e(size_t n)
{
    if (n < 3) {
        return n + 1;
    }

    size_t sieve_size = estimate_sieve_size(n);
    unsigned char *sieve = calloc((sieve_size + 7) / 8, sizeof(unsigned char));

    size_t prime_count = 1;
    size_t nth_prime = 0;

    for (size_t i = 2; i < sieve_size; i += 2) {
        sieve[i >> 3] |= (1 << (i & 7));
    }

    for (size_t i = 3; i < sieve_size; i += 2) {

        if (!((sieve[i >> 3] >> (i & 7)) & 1)) {
            prime_count++;

            /* Return nth prime */
            if (prime_count == n) {
                nth_prime = i;
                break;
            }

            /*
            * Mark multiples of i as composite, starting at i^2
            * since multiples of i less than i^2 have already been
            * handled by smaller primes
            */
            for (size_t j = i * i; j < sieve_size; j += i) {
                sieve[j >> 3] |= (1 << (j & 7));
            }
        }
    }

    free(sieve);
    return nth_prime;
}

int main()
{
    int p = 0;
    FILE *fp;
    fp = fopen("time_data.csv", "w+");
    if (fp == NULL) {
        perror("Failed to open file\n");
        return 1;
    }

    /* Warm up */
    for (int i = 1; i < 1000; i++) {
        p = sieve_of_e(i);
    }

    fprintf(fp, "N, Time, Prime\n");
    for (int i = 1000; i <= 10000; i += 1000) {
        clock_t start_time, end_time;
        double cpu_time_used;
        start_time = clock();
        p = sieve_of_e(i);
        end_time = clock();
        cpu_time_used = (double) (end_time - start_time) / CLOCKS_PER_SEC;
        fprintf(fp, "%d, %f, %d\n", i, cpu_time_used, p);
    }
    fclose(fp);
    return 0;
}