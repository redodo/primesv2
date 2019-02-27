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
#define _6n1_inc_stop(d, max_d, step)     d += step; if (d >= max_d) return true;
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

    // Check the number against some baked primes.
    _prime_check(n,  3,    9);
    _prime_check(n,  5,   25);
    _prime_check(n,  7,   49);
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
        //
        //      dividers    lines   reduction
        //      5           5       20.0%
        //      5 7         35      31.4%
        //  --> 5 7 11      385     37.7%     <--
        //      5 7 11 13   4250    42.5%
        //
        // Dashes are possible prime numbers.
        // Numbers mark division possible by number.
        // '------------------------------.--.
        _6n1_both(  n, d, max_d,  8); //  -  - ...1
        _6n1_right( n, d, max_d,  4); //  7  - ...2
        _6n1_left(  n, d, max_d,  6); //  -  5 ...3
        _6n1_left(  n, d, max_d,  8); //  -  7 ...4
        _6n1_right( n, d, max_d,  4); //  5  - ...5
        _6n1_both(  n, d, max_d,  6); //  -  - ...6
        _6n1_both(  n, d, max_d,  6); //  -  - ...7
        _6n1_left(  n, d, max_d, 14); //  -  5 ...8
        _6n1_none(                 ); //  7 11 ...9
        _6n1_right( n, d, max_d,  4); //  5  - ..10
        _6n1_left(  n, d, max_d,  6); //  -  7 ..11
        _6n1_both(  n, d, max_d, 12); //  -  - ..12
        _6n1_none(                 ); // 11  5 ..13
        _6n1_both(  n, d, max_d,  8); //  -  - ..14
        _6n1_right( n, d, max_d,  6); //  5  - ..15
        _6n1_right( n, d, max_d,  4); //  7  - ..16
        _6n1_both(  n, d, max_d,  6); //  -  - ..17
        _6n1_left(  n, d, max_d,  6); //  -  5 ..18
        _6n1_both(  n, d, max_d, 12); //  -  - ..19
        _6n1_none(                 ); //  5 11 ..20
        _6n1_both(  n, d, max_d,  6); //  -  - ..21
        _6n1_both(  n, d, max_d, 14); //  -  - ..22
        _6n1_none(                 ); //  7  5 ..23
        _6n1_right( n, d, max_d, 10); // 11  - ..24
        _6n1_none(                 ); //  5  7 ..25
        _6n1_both(  n, d, max_d,  6); //  -  - ..26
        _6n1_both(  n, d, max_d,  6); //  -  - ..27
        _6n1_left(  n, d, max_d,  6); //  -  5 ..28
        _6n1_both(  n, d, max_d,  8); //  -  - ..29
        _6n1_right( n, d, max_d,  4); //  5  - ..30
        _6n1_left(  n, d, max_d,  6); //  - 11 ..31
        _6n1_left(  n, d, max_d,  6); //  -  7 ..32
        _6n1_left(  n, d, max_d,  6); //  -  5 ..33
        _6n1_both(  n, d, max_d,  8); //  -  - ..34
        _6n1_right( n, d, max_d,  4); //  5  - ..35
        _6n1_both(  n, d, max_d,  8); //  -  - ..36
        _6n1_right( n, d, max_d,  4); //  7  - ..37
        _6n1_left(  n, d, max_d,  6); //  -  5 ..38
        _6n1_left(  n, d, max_d,  8); //  -  7 ..39
        _6n1_right( n, d, max_d,  4); //  5  - ..40
        _6n1_both(  n, d, max_d,  6); //  -  - ..41
        _6n1_left(  n, d, max_d,  6); //  - 11 ..42
        _6n1_left(  n, d, max_d,  8); //  -  5 ..43
        _6n1_right( n, d, max_d,  6); //  7  - ..44
        _6n1_right( n, d, max_d, 10); //  5  - ..45
        _6n1_none(                 ); // 11  7 ..46
        _6n1_both(  n, d, max_d,  6); //  -  - ..47
        _6n1_left(  n, d, max_d,  6); //  -  5 ..48
        _6n1_both(  n, d, max_d,  8); //  -  - ..49
        _6n1_right( n, d, max_d,  6); //  5  - ..50
        _6n1_right( n, d, max_d,  4); //  7  - ..51
        _6n1_both(  n, d, max_d,  6); //  -  - ..52
        _6n1_left(  n, d, max_d,  6); //  -  5 ..53
        _6n1_both(  n, d, max_d,  8); //  -  - ..54
        _6n1_right( n, d, max_d,  4); //  5  - ..55
        _6n1_both(  n, d, max_d,  8); //  -  - ..56
        _6n1_right( n, d, max_d, 10); // 11  - ..57
        _6n1_none(                 ); //  7  5 ..58
        _6n1_both(  n, d, max_d, 12); //  -  - ..59
        _6n1_none(                 ); //  5  7 ..60
        _6n1_both(  n, d, max_d,  6); //  -  - ..61
        _6n1_both(  n, d, max_d,  6); //  -  - ..62
        _6n1_left(  n, d, max_d,  6); //  -  5 ..63
        _6n1_left(  n, d, max_d,  8); //  - 11 ..64
        _6n1_right( n, d, max_d,  4); //  5  - ..65
        _6n1_both(  n, d, max_d,  6); //  -  - ..66
        _6n1_left(  n, d, max_d, 12); //  -  7 ..67
        _6n1_none(                 ); // 11  5 ..68
        _6n1_both(  n, d, max_d,  8); //  -  - ..69
        _6n1_right( n, d, max_d,  4); //  5  - ..70
        _6n1_both(  n, d, max_d,  8); //  -  - ..71
        _6n1_right( n, d, max_d,  4); //  7  - ..72
        _6n1_left(  n, d, max_d,  6); //  -  5 ..73
        _6n1_left(  n, d, max_d, 12); //  -  7 ..74
        _6n1_none(                 ); //  5 11 ..75
        _6n1_both(  n, d, max_d,  6); //  -  - ..76
        _6n1_both(  n, d, max_d,  6); //  -  - ..77
        _6n1_left(  n, d, max_d,  8); //  -  5 ..78
        _6n1_right( n, d, max_d,  6); //  7  - ..79
        _6n1_right( n, d, max_d,  4); //  5  - ..80
        _6n1_left(  n, d, max_d,  6); //  -  7 ..81
        _6n1_both(  n, d, max_d,  6); //  -  - ..82
        _6n1_left(  n, d, max_d,  6); //  -  5 ..83
        _6n1_both(  n, d, max_d,  8); //  -  - ..84
        _6n1_right( n, d, max_d, 10); //  5  - ..85
        _6n1_none(                 ); //  7 11 ..86
        _6n1_both(  n, d, max_d,  6); //  -  - ..87
        _6n1_left(  n, d, max_d,  6); //  -  5 ..88
        _6n1_both(  n, d, max_d,  8); //  -  - ..89
        _6n1_right( n, d, max_d,  4); //  5  - ..90
        _6n1_both(  n, d, max_d,  6); //  -  - ..91
        _6n1_both(  n, d, max_d, 12); //  -  - ..92
        _6n1_none(                 ); //  7  5 ..93
        _6n1_both(  n, d, max_d, 12); //  -  - ..94
        _6n1_none(                 ); //  5  7 ..95
        _6n1_both(  n, d, max_d,  6); //  -  - ..96
        _6n1_left(  n, d, max_d,  6); //  - 11 ..97
        _6n1_left(  n, d, max_d,  6); //  -  5 ..98
        _6n1_both(  n, d, max_d,  8); //  -  - ..99
        _6n1_right( n, d, max_d,  6); //  5  - .100
        _6n1_right( n, d, max_d,  4); // 11  - .101
        _6n1_left(  n, d, max_d,  6); //  -  7 .102
        _6n1_left(  n, d, max_d,  6); //  -  5 .103
        _6n1_both(  n, d, max_d,  8); //  -  - .104
        _6n1_right( n, d, max_d,  4); //  5  - .105
        _6n1_both(  n, d, max_d,  8); //  -  - .106
        _6n1_right( n, d, max_d,  4); //  7  - .107
        _6n1_left(  n, d, max_d,  6); //  -  5 .108
        _6n1_left(  n, d, max_d,  8); //  -  7 .109
        _6n1_right( n, d, max_d,  4); //  5  - .110
        _6n1_both(  n, d, max_d,  8); //  -  - .111
        _6n1_right( n, d, max_d,  4); // 11  - .112
        _6n1_left(  n, d, max_d,  8); //  -  5 .113
        _6n1_right( n, d, max_d,  6); //  7  - .114
        _6n1_right( n, d, max_d,  4); //  5  - .115
        _6n1_left(  n, d, max_d,  6); //  -  7 .116
        _6n1_both(  n, d, max_d,  6); //  -  - .117
        _6n1_left(  n, d, max_d,  6); //  -  5 .118
        _6n1_left(  n, d, max_d,  8); //  - 11 .119
        _6n1_right( n, d, max_d,  6); //  5  - .120
        _6n1_right( n, d, max_d,  4); //  7  - .121
        _6n1_both(  n, d, max_d, 12); //  -  - .122
        _6n1_none(                 ); // 11  5 .123
        _6n1_both(  n, d, max_d,  8); //  -  - .124
        _6n1_right( n, d, max_d,  4); //  5  - .125
        _6n1_both(  n, d, max_d,  6); //  -  - .126
        _6n1_both(  n, d, max_d, 12); //  -  - .127
        _6n1_none(                 ); //  7  5 .128
        _6n1_both(  n, d, max_d, 12); //  -  - .129
        _6n1_none(                 ); //  5  7 .130
        _6n1_both(  n, d, max_d,  6); //  -  - .131
        _6n1_both(  n, d, max_d,  6); //  -  - .132
        _6n1_left(  n, d, max_d,  8); //  -  5 .133
        _6n1_right( n, d, max_d,  6); // 11  - .134
        _6n1_right( n, d, max_d,  4); //  5  - .135
        _6n1_both(  n, d, max_d,  6); //  -  - .136
        _6n1_left(  n, d, max_d,  6); //  -  7 .137
        _6n1_left(  n, d, max_d,  6); //  -  5 .138
        _6n1_both(  n, d, max_d,  8); //  -  - .139
        _6n1_right( n, d, max_d,  4); //  5  - .140
        _6n1_left(  n, d, max_d,  8); //  - 11 .141
        _6n1_right( n, d, max_d,  4); //  7  - .142
        _6n1_left(  n, d, max_d,  6); //  -  5 .143
        _6n1_left(  n, d, max_d,  8); //  -  7 .144
        _6n1_right( n, d, max_d,  4); //  5  - .145
        _6n1_both(  n, d, max_d,  6); //  -  - .146
        _6n1_both(  n, d, max_d,  6); //  -  - .147
        _6n1_left(  n, d, max_d,  8); //  -  5 .148
        _6n1_right( n, d, max_d,  6); //  7  - .149
        _6n1_right( n, d, max_d,  4); //  5  - .150
        _6n1_left(  n, d, max_d,  6); //  -  7 .151
        _6n1_left(  n, d, max_d,  6); //  - 11 .152
        _6n1_left(  n, d, max_d,  6); //  -  5 .153
        _6n1_both(  n, d, max_d,  8); //  -  - .154
        _6n1_right( n, d, max_d,  6); //  5  - .155
        _6n1_right( n, d, max_d,  4); //  7  - .156
        _6n1_both(  n, d, max_d,  6); //  -  - .157
        _6n1_left(  n, d, max_d,  6); //  -  5 .158
        _6n1_both(  n, d, max_d,  8); //  -  - .159
        _6n1_right( n, d, max_d,  4); //  5  - .160
        _6n1_both(  n, d, max_d,  6); //  -  - .161
        _6n1_both(  n, d, max_d, 12); //  -  - .162
        _6n1_none(                 ); //  7  5 .163
        _6n1_both(  n, d, max_d, 12); //  -  - .164
        _6n1_none(                 ); //  5  7 .165
        _6n1_both(  n, d, max_d,  8); //  -  - .166
        _6n1_right( n, d, max_d,  4); // 11  - .167
        _6n1_left(  n, d, max_d,  6); //  -  5 .168
        _6n1_both(  n, d, max_d,  8); //  -  - .169
        _6n1_right( n, d, max_d,  4); //  5  - .170
        _6n1_both(  n, d, max_d,  6); //  -  - .171
        _6n1_left(  n, d, max_d,  6); //  -  7 .172
        _6n1_left(  n, d, max_d,  6); //  -  5 .173
        _6n1_left(  n, d, max_d,  8); //  - 11 .174
        _6n1_right( n, d, max_d,  4); //  5  - .175
        _6n1_both(  n, d, max_d,  8); //  -  - .176
        _6n1_right( n, d, max_d, 10); //  7  - .177
        _6n1_none(                 ); // 11  5 .178
        _6n1_left(  n, d, max_d,  8); //  -  7 .179
        _6n1_right( n, d, max_d,  4); //  5  - .180
        _6n1_both(  n, d, max_d,  6); //  -  - .181
        _6n1_both(  n, d, max_d,  6); //  -  - .182
        _6n1_left(  n, d, max_d,  8); //  -  5 .183
        _6n1_right( n, d, max_d, 10); //  7  - .184
        _6n1_none(                 ); //  5 11 .185
        _6n1_left(  n, d, max_d,  6); //  -  7 .186
        _6n1_both(  n, d, max_d,  6); //  -  - .187
        _6n1_left(  n, d, max_d,  8); //  -  5 .188
        _6n1_right( n, d, max_d,  6); // 11  - .189
        _6n1_right( n, d, max_d,  6); //  5  - .190
        _6n1_right( n, d, max_d,  4); //  7  - .191
        _6n1_both(  n, d, max_d,  6); //  -  - .192
        _6n1_left(  n, d, max_d,  6); //  -  5 .193
        _6n1_both(  n, d, max_d,  8); //  -  - .194
        _6n1_right( n, d, max_d,  4); //  5  - .195
        _6n1_left(  n, d, max_d,  6); //  - 11 .196
        _6n1_both(  n, d, max_d, 12); //  -  - .197
        _6n1_none(                 ); //  7  5 .198
        _6n1_both(  n, d, max_d, 12); //  -  - .199
        _6n1_none(                 ); //  5  7 .200
        _6n1_both(  n, d, max_d,  6); //  -  - .201
        _6n1_both(  n, d, max_d,  6); //  -  - .202
        _6n1_left(  n, d, max_d,  6); //  -  5 .203
        _6n1_both(  n, d, max_d,  8); //  -  - .204
        _6n1_right( n, d, max_d,  4); //  5  - .205
        _6n1_both(  n, d, max_d,  6); //  -  - .206
        _6n1_left(  n, d, max_d,  6); //  -  7 .207
        _6n1_left(  n, d, max_d,  6); //  -  5 .208
        _6n1_both(  n, d, max_d,  8); //  -  - .209
        _6n1_right( n, d, max_d,  6); //  5  - .210
        _6n1_right( n, d, max_d,  6); // 11  - .211
        _6n1_right( n, d, max_d,  4); //  7  - .212
        _6n1_left(  n, d, max_d,  6); //  -  5 .213
        _6n1_left(  n, d, max_d,  8); //  -  7 .214
        _6n1_right( n, d, max_d,  4); //  5  - .215
        _6n1_both(  n, d, max_d,  6); //  -  - .216
        _6n1_both(  n, d, max_d,  6); //  -  - .217
        _6n1_left(  n, d, max_d,  8); //  -  5 .218
        _6n1_right( n, d, max_d,  6); //  7  - .219
        _6n1_right( n, d, max_d,  4); //  5  - .220
        _6n1_left(  n, d, max_d,  8); //  -  7 .221
        _6n1_right( n, d, max_d,  4); // 11  - .222
        _6n1_left(  n, d, max_d,  6); //  -  5 .223
        _6n1_both(  n, d, max_d,  8); //  -  - .224
        _6n1_right( n, d, max_d,  6); //  5  - .225
        _6n1_right( n, d, max_d,  4); //  7  - .226
        _6n1_both(  n, d, max_d,  6); //  -  - .227
        _6n1_left(  n, d, max_d,  6); //  -  5 .228
        _6n1_left(  n, d, max_d,  8); //  - 11 .229
        _6n1_right( n, d, max_d,  4); //  5  - .230
        _6n1_both(  n, d, max_d,  6); //  -  - .231
        _6n1_both(  n, d, max_d, 12); //  -  - .232
        _6n1_none(                 ); //  7  5 .233
        _6n1_both(  n, d, max_d, 12); //  -  - .234
        _6n1_none(                 ); //  5  7 .235
        _6n1_both(  n, d, max_d,  6); //  -  - .236
        _6n1_both(  n, d, max_d,  6); //  -  - .237
        _6n1_left(  n, d, max_d,  6); //  -  5 .238
        _6n1_both(  n, d, max_d, 12); //  -  - .239
        _6n1_none(                 ); //  5 11 .240
        _6n1_both(  n, d, max_d,  6); //  -  - .241
        _6n1_left(  n, d, max_d,  6); //  -  7 .242
        _6n1_left(  n, d, max_d,  8); //  -  5 .243
        _6n1_right( n, d, max_d,  6); // 11  - .244
        _6n1_right( n, d, max_d,  4); //  5  - .245
        _6n1_both(  n, d, max_d,  8); //  -  - .246
        _6n1_right( n, d, max_d,  4); //  7  - .247
        _6n1_left(  n, d, max_d,  6); //  -  5 .248
        _6n1_left(  n, d, max_d,  8); //  -  7 .249
        _6n1_right( n, d, max_d,  4); //  5  - .250
        _6n1_left(  n, d, max_d,  6); //  - 11 .251
        _6n1_both(  n, d, max_d,  6); //  -  - .252
        _6n1_left(  n, d, max_d,  8); //  -  5 .253
        _6n1_right( n, d, max_d,  6); //  7  - .254
        _6n1_right( n, d, max_d,  4); //  5  - .255
        _6n1_left(  n, d, max_d,  6); //  -  7 .256
        _6n1_both(  n, d, max_d,  6); //  -  - .257
        _6n1_left(  n, d, max_d,  6); //  -  5 .258
        _6n1_both(  n, d, max_d,  8); //  -  - .259
        _6n1_right( n, d, max_d,  6); //  5  - .260
        _6n1_right( n, d, max_d,  4); //  7  - .261
        _6n1_left(  n, d, max_d,  6); //  - 11 .262
        _6n1_left(  n, d, max_d,  6); //  -  5 .263
        _6n1_both(  n, d, max_d,  8); //  -  - .264
        _6n1_right( n, d, max_d,  6); //  5  - .265
        _6n1_right( n, d, max_d,  4); // 11  - .266
        _6n1_both(  n, d, max_d, 12); //  -  - .267
        _6n1_none(                 ); //  7  5 .268
        _6n1_both(  n, d, max_d, 12); //  -  - .269
        _6n1_none(                 ); //  5  7 .270
        _6n1_both(  n, d, max_d,  6); //  -  - .271
        _6n1_both(  n, d, max_d,  6); //  -  - .272
        _6n1_left(  n, d, max_d,  6); //  -  5 .273
        _6n1_both(  n, d, max_d,  8); //  -  - .274
        _6n1_right( n, d, max_d,  4); //  5  - .275
        _6n1_both(  n, d, max_d, 12); //  -  - .276
        _6n1_none(                 ); // 11  7 .277
        _6n1_left(  n, d, max_d,  6); //  -  5 .278
        _6n1_both(  n, d, max_d,  8); //  -  - .279
        _6n1_right( n, d, max_d,  4); //  5  - .280
        _6n1_both(  n, d, max_d,  8); //  -  - .281
        _6n1_right( n, d, max_d,  4); //  7  - .282
        _6n1_left(  n, d, max_d,  6); //  -  5 .283
        _6n1_left(  n, d, max_d,  8); //  -  7 .284
        _6n1_right( n, d, max_d,  4); //  5  - .285
        _6n1_both(  n, d, max_d,  6); //  -  - .286
        _6n1_both(  n, d, max_d, 14); //  -  - .287
        _6n1_none(                 ); // 11  5 .288
        _6n1_right( n, d, max_d,  6); //  7  - .289
        _6n1_right( n, d, max_d,  4); //  5  - .290
        _6n1_left(  n, d, max_d,  6); //  -  7 .291
        _6n1_both(  n, d, max_d,  6); //  -  - .292
        _6n1_left(  n, d, max_d,  6); //  -  5 .293
        _6n1_both(  n, d, max_d, 14); //  -  - .294
        _6n1_none(                 ); //  5 11 .295
        _6n1_right( n, d, max_d,  4); //  7  - .296
        _6n1_both(  n, d, max_d,  6); //  -  - .297
        _6n1_left(  n, d, max_d,  8); //  -  5 .298
        _6n1_right( n, d, max_d,  6); // 11  - .299
        _6n1_right( n, d, max_d,  4); //  5  - .300
        _6n1_both(  n, d, max_d,  6); //  -  - .301
        _6n1_both(  n, d, max_d, 12); //  -  - .302
        _6n1_none(                 ); //  7  5 .303
        _6n1_both(  n, d, max_d, 12); //  -  - .304
        _6n1_none(                 ); //  5  7 .305
        _6n1_left(  n, d, max_d,  6); //  - 11 .306
        _6n1_both(  n, d, max_d,  6); //  -  - .307
        _6n1_left(  n, d, max_d,  6); //  -  5 .308
        _6n1_both(  n, d, max_d,  8); //  -  - .309
        _6n1_right( n, d, max_d,  4); //  5  - .310
        _6n1_both(  n, d, max_d,  6); //  -  - .311
        _6n1_left(  n, d, max_d,  6); //  -  7 .312
        _6n1_left(  n, d, max_d,  6); //  -  5 .313
        _6n1_both(  n, d, max_d,  8); //  -  - .314
        _6n1_right( n, d, max_d,  4); //  5  - .315
        _6n1_both(  n, d, max_d, 12); //  -  - .316
        _6n1_none(                 ); //  7 11 .317
        _6n1_left(  n, d, max_d,  6); //  -  5 .318
        _6n1_left(  n, d, max_d,  8); //  -  7 .319
        _6n1_right( n, d, max_d,  6); //  5  - .320
        _6n1_right( n, d, max_d,  4); // 11  - .321
        _6n1_both(  n, d, max_d,  6); //  -  - .322
        _6n1_left(  n, d, max_d,  8); //  -  5 .323
        _6n1_right( n, d, max_d,  6); //  7  - .324
        _6n1_right( n, d, max_d,  4); //  5  - .325
        _6n1_left(  n, d, max_d,  6); //  -  7 .326
        _6n1_both(  n, d, max_d,  6); //  -  - .327
        _6n1_left(  n, d, max_d,  6); //  -  5 .328
        _6n1_both(  n, d, max_d,  8); //  -  - .329
        _6n1_right( n, d, max_d,  6); //  5  - .330
        _6n1_right( n, d, max_d,  6); //  7  - .331
        _6n1_right( n, d, max_d,  4); // 11  - .332
        _6n1_left(  n, d, max_d,  6); //  -  5 .333
        _6n1_both(  n, d, max_d,  8); //  -  - .334
        _6n1_right( n, d, max_d,  4); //  5  - .335
        _6n1_both(  n, d, max_d,  6); //  -  - .336
        _6n1_both(  n, d, max_d, 12); //  -  - .337
        _6n1_none(                 ); //  7  5 .338
        _6n1_left(  n, d, max_d, 12); //  - 11 .339
        _6n1_none(                 ); //  5  7 .340
        _6n1_both(  n, d, max_d,  6); //  -  - .341
        _6n1_both(  n, d, max_d, 12); //  -  - .342
        _6n1_none(                 ); // 11  5 .343
        _6n1_both(  n, d, max_d,  8); //  -  - .344
        _6n1_right( n, d, max_d,  4); //  5  - .345
        _6n1_both(  n, d, max_d,  6); //  -  - .346
        _6n1_left(  n, d, max_d,  6); //  -  7 .347
        _6n1_left(  n, d, max_d,  6); //  -  5 .348
        _6n1_both(  n, d, max_d, 12); //  -  - .349
        _6n1_none(                 ); //  5 11 .350
        _6n1_both(  n, d, max_d,  8); //  -  - .351
        _6n1_right( n, d, max_d,  4); //  7  - .352
        _6n1_left(  n, d, max_d, 14); //  -  5 .353
        _6n1_none(                 ); // 11  7 .354
        _6n1_right( n, d, max_d,  4); //  5  - .355
        _6n1_both(  n, d, max_d,  6); //  -  - .356
        _6n1_both(  n, d, max_d,  6); //  -  - .357
        _6n1_left(  n, d, max_d,  8); //  -  5 .358
        _6n1_right( n, d, max_d,  6); //  7  - .359
        _6n1_right( n, d, max_d,  4); //  5  - .360
        _6n1_left(  n, d, max_d,  6); //  -  7 .361
        _6n1_both(  n, d, max_d,  6); //  -  - .362
        _6n1_left(  n, d, max_d,  6); //  -  5 .363
        _6n1_both(  n, d, max_d,  8); //  -  - .364
        _6n1_right( n, d, max_d,  6); //  5  - .365
        _6n1_right( n, d, max_d,  4); //  7  - .366
        _6n1_both(  n, d, max_d,  6); //  -  - .367
        _6n1_left(  n, d, max_d,  6); //  -  5 .368
        _6n1_both(  n, d, max_d,  8); //  -  - .369
        _6n1_right( n, d, max_d,  4); //  5  - .370
        _6n1_both(  n, d, max_d,  6); //  -  - .371
        _6n1_left(  n, d, max_d, 12); //  - 11 .372
        _6n1_none(                 ); //  7  5 .373
        _6n1_both(  n, d, max_d, 14); //  -  - .374
        _6n1_none(                 ); //  5  7 .375
        _6n1_right( n, d, max_d,  4); // 11  - .376
        _6n1_both(  n, d, max_d,  6); //  -  - .377
        _6n1_left(  n, d, max_d,  6); //  -  5 .378
        _6n1_both(  n, d, max_d,  8); //  -  - .379
        _6n1_right( n, d, max_d,  4); //  5  - .380
        _6n1_both(  n, d, max_d,  6); //  -  - .381
        _6n1_left(  n, d, max_d,  6); //  -  7 .382
        _6n1_left(  n, d, max_d,  6); //  -  5 .383
        _6n1_both(  n, d, max_d,  8); //  -  - .384
        _6n1_right( n, d, max_d,  4); //  5  - .385
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
