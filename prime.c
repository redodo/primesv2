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

// Increment the divider by step and stop when the max has been reached.
#define _6n1_inc_stop(d, max_d, step)      d += step; if (d >= max_d) return true;
// Check both 6n-1 and 6n+1. Do one inc_stop. Use step value 6 as base.
#define _6n1_both(n, d, max_d, step)      if ((n % d) == 0 || (n % (d + 2)) == 0) return false; _6n1_inc_stop(d, max_d, step)
// Check only 6n-1. Do one inc_stop. Use step value 6 as base.
#define _6n1_left(n, d, max_d, step)      if ((n % d) == 0) return false; _6n1_inc_stop(d, max_d, step)
// Check only 6n+1. Do one inc_stop. Use step value 4 as base. THE MACRO PRECEDING THIS MUST INCREASE ITS STEP BY 2.
#define _6n1_right(n, d, max_d, step)     if ((n % d) == 0) return false; _6n1_inc_stop(d, max_d, step)
// Empty macro. Increase preceding macro step by 6.
#define _6n1_none()

bool is_prime_fast(uint64_t n) {
    // An even number is only prime when it is 2.
    if ((n & 1) == 0) {
        return n == 2;
    }
    // An uneven number of 3 or lower is only prime when it is 3.
    if (n <= 3 ) {
        return n == 3;
    }
    _prime_check(n,  3,    9);
    _prime_check(n,  5,   25);
    _prime_check(n,  7,   49);

    // A prime number larger than 3 can always be written as 6n±1.
    //
    // We can return false if the number is not next to a multiple of 6. Which
    // can be checked by doing a mod 6 on the number. The output should be 1 or 5:
    //
    //     i        | ...  3   4   5   6   7   8   9  10  11  12  13  ...
    //     i mod 6  | ...  3   4   5   0   1   2   3   4   5   0   1  ...
    //                             ^       ^               ^       ^
    //                           6*1-1   6*1+1           6*2-1   6*2+1
    //
    // At this point however, i mod 6 is always 1 or 5, because we eliminated all
    // multiples of 2 and 3.  But, somehow, keeping this code here saves 83ms when
    // checking the first 10,000,000 numbers for primality...
    uint8_t mod = n % 6;
    if (mod != 1 && mod != 5) {
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
    uint64_t d = 5;
    while (true) {
        // We skip division with multiples of 5 and 7.
        // Which is why we have this hardcoded loop of 35 operations (5 * 7)
        //
        // This further limits the numbers we divide with by 31%
        //
        // Skipping 11 too would require a loop of 385 operations (5 * 7 * 11)
        //
        // Dashes are possible prime numbers. Numbers mark division.
        // '------------------------------.
        _6n1_both(  n, d, max_d,  8); // - - ...1
        _6n1_right( n, d, max_d,  4); // 7 - ...2
        _6n1_left(  n, d, max_d,  6); // - 5 ...3
        _6n1_left(  n, d, max_d,  8); // - 7 ...4
        _6n1_right( n, d, max_d,  4); // 5 - ...5
        _6n1_both(  n, d, max_d,  6); // - - ...6
        _6n1_both(  n, d, max_d,  6); // - - ...7
        _6n1_left(  n, d, max_d,  8); // - 5 ...8
        _6n1_right( n, d, max_d,  6); // 7 - ...9
        _6n1_right( n, d, max_d,  4); // 5 - ..10
        _6n1_left(  n, d, max_d,  6); // - 7 ..11
        _6n1_both(  n, d, max_d,  6); // - - ..12
        _6n1_left(  n, d, max_d,  6); // - 5 ..13
        _6n1_both(  n, d, max_d,  8); // - - ..14
        _6n1_right( n, d, max_d,  6); // 5 - ..15
        _6n1_right( n, d, max_d,  4); // 7 - ..16
        _6n1_both(  n, d, max_d,  6); // - - ..17
        _6n1_left(  n, d, max_d,  6); // - 5 ..18
        _6n1_both(  n, d, max_d,  8); // - - ..19
        _6n1_right( n, d, max_d,  4); // 5 - ..20
        _6n1_both(  n, d, max_d,  6); // - - ..21
        _6n1_both(  n, d, max_d, 12); // - - ..22
        _6n1_none(                 ); // 7 5 ..23
        _6n1_both(  n, d, max_d, 12); // - - ..24
        _6n1_none(                 ); // 5 7 ..25
        _6n1_both(  n, d, max_d,  6); // - - ..26
        _6n1_both(  n, d, max_d,  6); // - - ..27
        _6n1_left(  n, d, max_d,  6); // - 5 ..28
        _6n1_both(  n, d, max_d,  8); // - - ..29
        _6n1_right( n, d, max_d,  4); // 5 - ..30
        _6n1_both(  n, d, max_d,  6); // - - ..31
        _6n1_left(  n, d, max_d,  6); // - 7 ..32
        _6n1_left(  n, d, max_d,  6); // - 5 ..33
        _6n1_both(  n, d, max_d,  8); // - - ..34
        _6n1_right( n, d, max_d,  4); // 5 - ..35
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
    run_prime_fast(10000000);
    return 0;
}
