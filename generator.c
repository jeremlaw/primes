/**************************************************************
 *
 *                     generator.c
 *
 *     This file contains the implementations for prime number
 *     generating functions
 *
 **************************************************************/
#include <stdbool.h>
#include "generator.h"
#include "dynamic_array.h"
#include "bit_array.h"
#include <math.h>

#include <stdio.h>

/********** isPrime ********
 *
 *      Helper for Brute force approach
 * 
 *      Checks for factors up to √p
 * 
 *      Time Complexity: O(√p)
 *      
 ******************************/
static bool isPrime(int p)
{
    /* 2 and 3 are prime (p is never 0 or 1)*/
    if (p < 4) {
        return true;
    }

    /* Check for factors up to √p */
    for (int i = 2; i * i <= p; i++) {

        /* Not prime if a factor is found */
        if (p % i == 0) {
            return false;
        }
    }
    return true;
}

/********** brute ********
 *
 *      Brute force approach for computing nth prime
 *      (~34 seconds to compute 1 millionth prime)
 * 
 *      Method: Finds first n prime numbers by checking
 *      each number p for factors up to √p
 * 
 *      Time Complexity: If p(n) is the nth prime number,
 *      O(p(n)(√p(n)) = O((n log n)^3/2)
 * 
 *      Space Complexity: O(1)
 *      
 ******************************/
int brute(int n)
{
    /* 1st prime is 2 */
    int p = 2;

    /* Iterate through first n primes */
    for (int i = 1; i < n; i++) {
        p++;

        /* Increase p until it is prime */
        while (!isPrime(p)) {
            p++;
        }
    }

    return p;
}

/********** prime_factors ********
 *
 *      Helper for improved prime generator
 * 
 *      Checks for prime factors up to √p
 * 
 *      Time Complexity: O((π(√p)) = O(√p / log √p)
 *      
 ******************************/
static bool prime_factors(Array array, int p)
{
    int curr_prime = 0;

    /* Get direct access to array for faster access time */
    int *arr = getArr(array);

    /* Check prime numbers up to sqrt(p) */
    for (int i = 0; curr_prime * curr_prime < p; i++) {
        curr_prime = arr[i];

        /* Not prime if a prime factor is found */
        if (p % curr_prime == 0) {
            return false;
        }
    }
    return true;
}

/********** generator ********
 *
 *      Improvement upon brute force approach
 *      ~1.2 seconds to compute 1 millionth prime
 *      ~30.5 seconds to compute 10 millionth prime
 * 
 *      Method: Stores first n prime numbers in dynamic array,
 *      checking each number p for prime factors up to √p
 * 
 *      Modifications: First 2 primes (2, 3) already stored so
 *      that p can be incremented by 2 (p is always odd)
 *      
 *      All primes previouly computed in the query loop are stored,
 *      meaning constant runtime if a larger prime has been found
 *      
 *      Time Complexity: If p(n) is the nth prime number,
 *      O(p(n) π(√p(n))) = O(n^3/2 log n))
 * 
 *      O(1) if a larger prime has been computed
 *      
 *      Space Complexity: O(n)
 *      
 ******************************/
int generator(Array primes, int n)
{
    /* 1st and 2nd primes already stored */
    if (n < 3) {
        return Array_at(primes, n - 1);
    }
    /* Get highest previous value of n */
    int max_prev_n = Array_size(primes);
    int p = 0;

    /* If only 1 prime has been found, start at p = 3 */
    if (max_prev_n == 2) {
       p = 3;
    }
    /* If <= n primes are already stored, return nth prime */
    if (n <= max_prev_n) {
        return Array_at(primes, n - 1);
    }

    /* Start at p = max prime stored */
    p = Array_at(primes, max_prev_n - 1);

    /* Increase p by 2 until n primes stored  */
    while(Array_size(primes) < n) {
        p += 2;

        /* Increase p until it is prime */
        while (!prime_factors(primes, p)) {
            p += 2;
        }

        /* Once a prime has been found, store it */
        Array_push(primes, p);
    }
    return p;
}

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
 *      ~0.11 seconds to compute 1 millionth prime
 *      ~1.4 seconds to compute 10 millionth prime
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
int sieve_of_e(size_t n)
{
    if (n < 3) {
        return n + 1;
    }

    size_t sieve_size = estimate_sieve_size(n);
    BitArray *bit_array = BitArray_new(sieve_size);

    size_t prime_count = 1;
    int nth_prime = 0;

    for (size_t i = 2; i < sieve_size; i += 2) {
        BitArray_set(bit_array, i);
    }

    for (size_t i = 3; i < sieve_size; i += 2) {

        if (!BitArray_get(bit_array, i)) {
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
                BitArray_set(bit_array, j);
            }
        }
    }

    BitArray_free(bit_array);
    return nth_prime;
}

/* Currently in progress */
int sieve_of_a(size_t n)
{
    if (n <= 0) return -1; // Invalid input
    
    // Estimate the upper limit for the nth prime
    size_t limit = estimate_sieve_size(n) * 1.1; // Add margin
    BitArray *isPrime = BitArray_new(limit + 1);

    size_t sqrtLimit = (size_t)sqrt((double)limit);

    // Sieve of Atkin
    for (size_t x = 1; x <= sqrtLimit; x++) {
        for (size_t y = 1; y <= sqrtLimit; y++) {
            size_t m = (4 * x * x) + (y * y);
            if (m <= limit && (m % 12 == 1 || m % 12 == 5)) {
                BitArray_toggle(isPrime, m);
            }

            m = (3 * x * x) + (y * y);
            if (m <= limit && m % 12 == 7) {
                BitArray_toggle(isPrime, m);
            }

            m = (3 * x * x) - (y * y);
            if (x > y && m <= limit && m % 12 == 11) {
                BitArray_toggle(isPrime, m);
            }
        }
    }

    for (size_t i = 0; i <= limit; i++) {
        if (BitArray_get(isPrime, i)) {
            printf("%zu is marked as prime\n", i);
        }
    }

    // Eliminate multiples of squares
    for (size_t i = 5; i <= sqrtLimit; i++) {
        if (BitArray_get(isPrime, i)) {
            for (size_t j = i * i; j <= limit; j += i * i) {
                BitArray_clear(isPrime, j);
            }
        }
    }

    for (size_t i = 0; i <= limit; i++) {
        if (BitArray_get(isPrime, i)) {
            printf("%zu is marked as prime\n", i);
        }
    }

    // Count primes to find the nth one
    size_t count = 0;
    for (size_t i = 2; i <= limit; i++) { // Include 2 and 3 directly
        if (i == 2 || i == 3 || BitArray_get(isPrime, i)) {
            count++;
            printf("Prime #%zu: %zu\n", count, i);
            if (count == n) {
                BitArray_free(isPrime);
                return i; // Found the nth prime
            }
        }
    }

    for (size_t i = 0; i <= limit; i++) {
        if (BitArray_get(isPrime, i)) {
            printf("%zu is marked as prime\n", i);
        }
    }

    // Cleanup and fallback
    BitArray_free(isPrime);
    return -1; // Should not happen if limit is large enough
}

// // Sieve of Atkin core logic
    // for (int x = 1; x <= sqrtLimit; x++) {
    //     for (int y = 1; y <= sqrtLimit; y++) {
    //         int n = (4 * x * x) + (y * y);
    //         if (n <= limit && (n % 12 == 1 || n % 12 == 5)) {
    //             BitArray_toggle(isPrime, n);
    //         }

    //         n = (3 * x * x) + (y * y);
    //         if (n <= limit && n % 12 == 7) {
    //             BitArray_toggle(isPrime, n);
    //         }

    //         n = (3 * x * x) - (y * y);
    //         if (x > y && n <= limit && n % 12 == 11) {
    //             BitArray_toggle(isPrime, n);
    //         }

    //          // Eliminate multiples of squares
    //         for (int i = 5; i <= sqrtLimit; i++) {
    //             if (BitArray_get(isPrime, i)) {
    //                 for (int j = i * i; j <= limit; j += i * i) {
    //                     BitArray_clear(isPrime, j);
    //                 }
    //             }
    //         }
    //     }
    // }