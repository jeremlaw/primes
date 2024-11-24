#include <stdio.h>
#include "generator.h"

int main(int argc, char *argv[])
{
    printf("Give me n, and I will give you the nth prime\n");
    printf("Enter 0 to quit\n");
    int n = 1;
    Array primes = Prime_Array();
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
        //int p = generator(primes, n);
        int p = sieve(n);
        printf("The %d%s prime is %d\n", n, suffix, p);
    }
    
    Array_free(primes);
    return 0;
}