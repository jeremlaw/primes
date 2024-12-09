#include <stdio.h>
#include <string.h>
#include "generator.h"

int main(int argc, char *argv[])
{
    fprintf(stderr, "\nWelcome to the Prime Number Factory\n");
    fprintf(stderr, "Which would you like to use?\n");
    fprintf(stderr, "(1) The brute force generator\n");
    fprintf(stderr, "(2) Our improved version\n");
    fprintf(stderr, "(3) The Sieve of Eratosthenes\n");
    fprintf(stderr, "Please choose 1, 2, or 3.\n");
    int a = 0;
    scanf("%d", &a);
    if (a < 1 || a > 3) {
        fprintf(stderr, "Default: Sieve of Eratosthenes\n");
    }
    Array primes = NULL;
    if (a == 2) {
        primes = Prime_Array();
    }
    fprintf(stderr, "Give me n, and I will give you the nth prime\n");
    fprintf(stderr, "Enter 0 to quit\n");
    int n = 1;
    while (n > 0) {
        scanf("%d", &n);
        if (n == 0) {
            return 0;
        }
        char *suffix = "  ";
        switch (n % 10) {
            case 1:
                suffix = "st";
                break;
            case 2:
                suffix = "nd";
                break;
            case 3:
                suffix = "rd";
                break;
            default:
                suffix = "th";
                break;
        }
        if ((n % 100) - (n % 10) == 10) {
            suffix = "th";
        }
        int p;
        if (primes != NULL) {
            p = generator(primes, n);
        } else if (a == 1) {
            p = brute(n);
        } else if (a == 3) {
            p = sieve_of_e(n);
        } else {
            fprintf(stderr, "Default: Sieve of Eratosthenes\n");
        }
        printf("The %d%s prime is %d\n", n, suffix, p);
    }
    
    if (primes != NULL) {
        Array_free(primes);
    }

    return 0;
}