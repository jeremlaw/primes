/**************************************************************
 *
 *                     generator.c
 *
 *     This file contains the implementation for prime number
 *     generating functions
 *
 **************************************************************/
#include <math.h>
#include <stdbool.h>
#include "generator.h"
#include "dynamic_array.h"
#include <stdio.h>

static bool isPrime(int p)
{
    /* 2 & 3 are prime (0 & 1 are never n)*/
    if (p < 4) {
        return true;
    }

    /* Check for factors up to sqrt(n) */
    for (int i = 2; i <= round(sqrt(p)); i++) {

        /* Not prime if a factor is found */
        if (p % i == 0) {
            return false;
        }
    }
    return true;
}

/* Brute force generator ~34 seconds to compute 1 millionth prime */
/* Runtime: O(n * sqrt n) = O(n^2) */
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

/* Check for prime factors up to sqrt(p) */
static bool prime_checker(Array array, int p)
{
    int curr_prime = 0;

    /* Check prime numbers up to sqrt(p) */
    int max = (int)sqrt(p);

    /* Get direct access to array for faster access time */
    int *arr = getArr(array);
    for (int i = 0; curr_prime < max; i++) {
        curr_prime = arr[i];

        /* Not prime if a prime factor is found */
        if (p % curr_prime == 0) {
            return false;
        }
    }
    return true;
}

/* Improved generator ~1.2 seconds to compute 1 millionth prime */
/* Checks for prime factors up to sqrt(n), keeps track of all */
/* previous primes found, including past inputs for n */
/* Runtime: O(n * sqrt(n / log n)) = O(n^2) */
/* Much faster if a close prime has already been found */
/* O(1) if larger prime has already been found */
int generator(Array primes, int n)
{
    if (n < 3) {
        return Array_at(primes, n - 1);
    }
    /* Get highest previous value of n */
    int max_prev_n = Array_size(primes);
    int p = 0;

    /* If only 1 prime has been found, start at p = 3 */
    /* Note: primes array is initialized with 2 and 3 in it */
    if (max_prev_n == 2) {
       p = 3;
    }

    /* If <= n primes are already stored, return nth prime */
    /* Note: this includes n = 1 */
    if (n <= max_prev_n) {
        return Array_at(primes, n - 1);
    }

    /* Start at p = max prime stored */
    p = Array_at(primes, max_prev_n - 1);

    /* Increase p by 2 until n primes stored  */
    while(Array_size(primes) < n) {
        p += 2;

        /* Increase p until it is prime */
        while (!prime_checker(primes, p)) {
            p += 2;
        }

        /* Once a prime has been found, store it */
        Array_push(primes, p);
    }
    return p;
}