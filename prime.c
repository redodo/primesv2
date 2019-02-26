#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define _prime_check(n,p,m)   if (n < m) return true; if (n % p == 0) return false;

bool is_prime_slow(uint64_t n) {
    if ((n & 1) == 0) {
        return n == 2;
    }
    if (n <= 3 ) {
        return n == 3;
    }
    uint64_t max_d = (uint64_t) sqrt((long double) n) + 1;
    for (uint64_t d = 3; d < max_d; d += 2) {
        if ((n % d) == 0) {
            return false;
        }
    }
    return true;
}

#define _6n1_addstop(d, max_d)   d += 6; if (d >= max_d) return true;
#define _6n1_both(n, d, max_d)   if ((n % d) == 0 || (n % (d + 2)) == 0) return false; _6n1_addstop(d, max_d)
#define _6n1_left(n, d, max_d)   if ((n % d) == 0) return false; _6n1_addstop(d, max_d)
#define _6n1_right(n, d, max_d)  if ((n % (d + 2)) == 0) return false; _6n1_addstop(d, max_d)
#define _6n1_none(n, d, max_d)   _6n1_addstop(d, max_d)

bool is_prime_fast(uint64_t n) {
    // An even number is only prime when it is 2.
    if ((n & 1) == 0) {
        return n == 2;
    }
    // An uneven number of 3 or lower is only prime when it is 3.
    if (n <= 3 ) {
        return n == 3;
    }
    // Check the number against 3, 5 and 7.
    _prime_check(n,  3,    9);
    _prime_check(n,  5,   25);
    _prime_check(n,  7,   49);
    // A number larger than 3 is only prime when it can be written as 6n±1.
    if ((n % 6 & 3) != 1) {
        return false;
    }
    // Check the number against all primes from 11 until 67.
    _prime_check(n, 11,  121);
    _prime_check(n, 13,  169);
    _prime_check(n, 17,  289);
    _prime_check(n, 19,  361);
    _prime_check(n, 23,  529);
    _prime_check(n, 29,  841);
    _prime_check(n, 31,  961);
    _prime_check(n, 37, 1369);
    _prime_check(n, 41, 1681);
    _prime_check(n, 43, 1849);
    _prime_check(n, 47, 2209);
    _prime_check(n, 53, 2809);
    _prime_check(n, 59, 3481);
    _prime_check(n, 61, 3721);
    _prime_check(n, 67, 4489);

    // Exhaust all other options.
    uint64_t max_d = (uint64_t) sqrt((long double) n) + 1;
    uint64_t d = 71;
    while (true) {
        // We skip division with multiples of 5 and 7.
        // Which is why we have this hardcoded loop of 35 operations (5 * 7)
        //
        // Skipping 11 too would require a loop of 385 operations (5 * 7 * 11) 
        _6n1_both(n, d, max_d);
        _6n1_right(n, d, max_d);
        _6n1_left(n, d, max_d);
        _6n1_left(n, d, max_d);
        _6n1_right(n, d, max_d);

        _6n1_both(n, d, max_d);
        _6n1_both(n, d, max_d);
        _6n1_left(n, d, max_d);
        _6n1_right(n, d, max_d);
        _6n1_right(n, d, max_d);

        _6n1_left(n, d, max_d);
        _6n1_both(n, d, max_d);
        _6n1_left(n, d, max_d);
        _6n1_both(n, d, max_d);
        _6n1_right(n, d, max_d);

        _6n1_right(n, d, max_d);
        _6n1_both(n, d, max_d);
        _6n1_left(n, d, max_d);
        _6n1_both(n, d, max_d);
        _6n1_right(n, d, max_d);

        _6n1_both(n, d, max_d);
        _6n1_both(n, d, max_d);
        _6n1_none(n, d, max_d);
        _6n1_both(n, d, max_d);
        _6n1_none(n, d, max_d);

        _6n1_both(n, d, max_d);
        _6n1_both(n, d, max_d);
        _6n1_left(n, d, max_d);
        _6n1_both(n, d, max_d);
        _6n1_right(n, d, max_d);

        _6n1_both(n, d, max_d);
        _6n1_left(n, d, max_d);
        _6n1_left(n, d, max_d);
        _6n1_both(n, d, max_d);
        _6n1_right(n, d, max_d);
    }
}

void test_prime_fast(uint64_t range) {
    for (uint64_t i = 0; i < range; i++) {
        bool slow = is_prime_slow(i);
        bool fast = is_prime_fast(i);
        if (slow != fast) {
            printf("Slow and fast disagreed on %d: slow had %d, fast had %d\n", i, slow, fast);
            return;
        }
    }
    printf("The fast function is accurate until atleast %d\n", range);
}

void run_prime_fast(uint64_t range) {
    for (uint64_t i = 0; i < range; i++) {
        bool fast = is_prime_fast(i);
    }
}

int main() {
    //test_prime_fast(10000000);
    run_prime_fast(5000000);
    return 0;
}
