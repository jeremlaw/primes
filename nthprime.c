#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <stdint.h>
#include <math.h>
#include <time.h>

/********** estimate_sieve_size ********
 *
 *      Helper function to calculate the estimated sieve size,
 *      which is the number of primes less than the nth prime
 * 
 *      Uses n (log n + log log n) as a knowd upper bound on 
 *      nth prime, which holds for n >= 6
 * 
 *      For n < 6, the upper bound is 12 (1 more than the 5th prime)
 * 
 *      Time Complexity: O(1)
 *      
 ******************************/
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

/********** sieve_of_e ********
 *      
 *      Uses Sieve of Eratosthenes to compute nth prime
 *      ~0.04 seconds to compute 1 millionth prime
 *      ~0.77 seconds to compute 10 millionth prime
 *      ~12.5 seconds to compute 100 millionth prime
 * 
 *      Method: Estimates nth prime, p(n), as n log(n log n)
 * 
 *      Each number up to the nth prime is marked '0' if prime
 *      and '1' if composite in an array of p(n) bits.
 *      
 *      Marks composites by checking multiples of each prime, p,
 *      starting p^2, since multiples less than p^2 have been
 *      handled by smaller primes. This eliminates the chance of
 *      marking the same composite twice.
 *      
 *      Time Complexity: O(n log n log log n)
 *      Space Complexity: O(n log n)
 *      
 ******************************/
static size_t sieve_of_e(size_t n)
{
    if (n < 3) {
        return n + 1;
    }

    size_t sieve_size = estimate_sieve_size(n);
    unsigned char *sieve = calloc((sieve_size + 7) / 8, sizeof(unsigned char));

    size_t prime_count = 1;
    size_t nth_prime = 0;

    /* Set even index bits to '1' */
    for (size_t i = 2; i < sieve_size; i += 2) {

        sieve[i >> 3] |= (1 << (i & 7));    /* Set i-th bit to '1' */
    }

    for (size_t i = 3; i < sieve_size; i += 2) {

        if (!((sieve[i >> 3] >> (i & 7)) & 1)) {    /* If i-th bit is '0' */
            prime_count++;

            /* Return nth prime */
            if (prime_count == n) {
                nth_prime = i;
                break;
            }

            /*
             * Mark multiples of i as composite, starting at i^2
             * since smaller multiples of have already been marked
             */
            size_t inc = i * 2;
            for (size_t j = i * i; j < sieve_size; j += inc) {

                sieve[j >> 3] |= (1 << (j & 7));    /* Set j-th bit to '1' */
            }
        }
    }

    free(sieve);
    return nth_prime;
}

int main(int argc, char *argv[])
{
    size_t n = 0;
    scanf("%zu", &n);

    if (n < 1) {
        exit(EXIT_FAILURE);
    }
    size_t p;
    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();
    p = sieve_of_e(n);
    end_time = clock();
    cpu_time_used = (double) (end_time - start_time) / CLOCKS_PER_SEC;

    printf("Prime: %zu\nTook %f seconds.\n", p, cpu_time_used);
    return 0;
}