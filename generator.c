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
    /* 2 and 3 are prime (0 & 1 are never p)*/
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

/********** isPrime ********
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

/********** isPrime ********
 *
 *      Helper function to calculate the estimated sieve size,
 *      which is the number of primes less than the nth prime
 * 
 *      Uses upper bound on nth prime ~ n log(n log n)
 * 
 *      Approximation is worse for small n, so a small
 *      constant approximation is used for small n
 * 
 *      Time Complexity: O(1)
 *      
 ******************************/
static size_t estimate_sieve_size(size_t n)
{
    /* Small constant for small n */
    if (n < 6) {
        return 15;
    }

    /* Estimated upper bound for nth prime */
    double estimate = n * log(n * log(n));
    return (size_t)ceil(estimate);
}

/********** sieve ********
 *
 *      Uses Sieve of Eratosthenes to compute nth prime
 *      ~0.14 seconds to compute 1 millionth prime
 *      ~2.1 seconds to compute 10 millionth prime
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
int sieve(size_t n)
{
    size_t sieve_size = estimate_sieve_size(n);
    BitArray *bit_array = BitArray_new(sieve_size);

    size_t prime_count = 0;
    int nth_prime = 0;

    for (size_t i = 2; i < sieve_size; i++) {

        /* If i is prime, increas prime count */
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