#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define baked_prime_check(n,p,m)   if (m > n) return true; if (n % p == 0) return false;

bool is_prime_fast(uint64_t n) {
    // An even number is only prime when it is 2.
    if ((n & 1) == 0) {
        return n == 2;
    }
    // An uneven number of 3 or lower is only prime when it is 3.
    if (n <= 3) {
        return n == 3;
    }
    // Prime numbers larger than 3 are a result of 6n±1.
    //
    // If neither number next to the number is divisible by 6, then
    // the number is not prime.
    //
    // The following is an optimized implementation to check if the number is
    // in 6n±1::
    //
    //    ((n % 6) & 1) != 1
    //
    // First. `n % 6` should be 1 or 5, because then the number is next to a number
    // divisible by 6, for a number divisible by 6 would have a modulo 6 result of 0.
    //
    // We can combine the check for 1 and 5 by applying a binary mask::
    //
    //     0000 : 0
    //     0001 : 1
    //     0010 : 2
    //     0011 : 3
    //     0100 : 4
    //     0101 : 5
    if ((n % 6 & 3) != 1) {
        return false;
    }
    baked_prime_check(n,  3,    9);
    baked_prime_check(n,  5,   25);
    baked_prime_check(n,  7,   49);
    baked_prime_check(n, 11,  121);
    baked_prime_check(n, 13,  169);
    baked_prime_check(n, 17,  289);
    baked_prime_check(n, 19,  361);
    baked_prime_check(n, 23,  529);
    baked_prime_check(n, 29,  841);
    baked_prime_check(n, 31,  961);
    baked_prime_check(n, 37, 1369);
    baked_prime_check(n, 41, 1681);
    baked_prime_check(n, 43, 1849);
    baked_prime_check(n, 47, 2209);
    baked_prime_check(n, 53, 2809);
    baked_prime_check(n, 59, 3481);
    baked_prime_check(n, 61, 3721);
    return true;
}

int main() {
    for (uint64_t i = 0; i < 70; i++) {
        bool p = is_prime_fast(i);
        printf("%d:%d\n", p, i);
        //if ((((i - 1) % 6)
    }
    return 0;
}
