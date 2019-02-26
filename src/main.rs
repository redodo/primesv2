#![feature(test)]
extern crate test;

use pbr::ProgressBar;

const INITIAL_DIVIDERS: [(u64, u64); 17] = [
    (3, 9),
    (5, 25),
    (7, 49),
    (11, 121),
    (13, 169),
    (17, 289),
    (19, 361),
    (23, 529),
    (29, 841),
    (31, 961),
    (37, 1369),
    (41, 1681),
    (43, 1849),
    (47, 2209),
    (53, 2809),
    (59, 3481),
    (61, 3721),
];
const INITIAL_DIVIDERS_ALT: [(u64, u64); 14] = [
    (11, 121),
    (13, 169),
    (17, 289),
    (19, 361),
    (23, 529),
    (29, 841),
    (31, 961),
    (37, 1369),
    (41, 1681),
    (43, 1849),
    (47, 2209),
    (53, 2809),
    (59, 3481),
    (61, 3721),
];

#[allow(dead_code)]
fn is_prime(n: u64) -> bool {
    if n & 1 == 0 {
        return n == 2;
    }
    if n == 3 {
        return true;
    }
    if n < 2 {
        return false;
    }

    for (d, dxd) in INITIAL_DIVIDERS.iter() {
        if dxd > &n {
            return true;
        }
        if n % d == 0 {
            return false;
        }
    }

    let max_d: u64 = (n as f64).sqrt() as u64 + 1;
    for d in (65..max_d).step_by(6) {
        if n % d == 0 || n % (d + 2) == 0 {
            return false;
        }
    }
    true
}

/// Bakes a precomputed primelist check into the code to save time doing list iteration.
macro_rules! precomputed_prime_list_check {
    ( $n:expr, [ $( ( $d:expr, $dxd:expr ) ),+ ] ) => {
        $(
            if $dxd > &$n { return true; }
            if $n % $d == 0 { return false; }
        )*
    };
}

#[allow(dead_code)]
fn is_prime_fast(n: u64) -> bool {
    // An even number is only prime when it is 2.
    if n & 1 == 0 {
        return n == 2;
    }
    // An uneven number below 4 is only prime when it is 3.
    if n <= 3 {
        return n == 3;
    }

    // First we check the number against 3, 5, and 7
    // Check against 3.
    if n < 9 { return true; }
    if n % 3 == 0 { return false; }
    // Check against 5.
    if n < 25 { return true; }
    if n % 5 == 0 { return false; }
    // Check against 7.
    if n < 49 { return true; }
    if n % 7 == 0 { return false; }

    // Prime numbers larger than 3 are a result of 6n±1.
    if n % 6 & 3 != 1 {
        return false;
    }
    /*
    precomputed_prime_list_check!(n, [
        ( 3,    9),
        ( 5,   25),
        ( 7,   49),
        (11,  121),
        (13,  169),
        (17,  289),
        (19,  361),
        (23,  529),
        (29,  841),
        (31,  961),
        (37, 1369),
        (41, 1681),
        (43, 1849),
        (47, 2209),
        (53, 2809),
        (59, 3481),
        (61, 3721),
    ]);
    */
    for (d, dxd) in INITIAL_DIVIDERS_ALT.iter() {
        if dxd > &n {
            // The current prime divider squared is larger than the number
            // It is prime.
            return true;
        }
        if n % d == 0 {
            return false;
        }
    }
    let max_d: u64 = (n as f64).sqrt() as u64 + 1;
    for d in (65..max_d).step_by(6) {
        if n % d == 0 || n % (d + 2) == 0 {
            return false;
        }
    }
    // We can not prove that the number is not prime.
    //   It is prime.
    true
}

#[allow(dead_code)]
fn reduce_i(n: u64) -> u64 {
    if n == 0 {
        return 0;
    }
    let m = n % 9;
    if m == 0 {
        return 9;
    }
    m
}

#[allow(dead_code)]
fn is_reduce_div(n: u64) -> bool {
    if n == 0 {
        return false;
    }
    n % reduce_i(n) == 0
}

#[allow(dead_code)]
fn split_n(mut n: u64) -> Vec<u64> {
    let mut split = Vec::new();
    loop {
        split.insert(0, n % 10);
        n /= 10;
        if n == 0 {
            break;
        }
    }
    split
}

#[allow(dead_code)]
fn do_prime_fn() {
    let max = 5000000;
    for i in 0..max {
        is_prime_fast(i);
    }
}

#[allow(dead_code)]
fn do_mod_test() {
    let max = 50;
    let a = 5;
    let b = 7;

    for i in 0..max {
        println!("{:>4} : {} : {}", i, i % a, i % b % a);
     }
}

fn main() {
    do_prime_fn();
    // do_mod_test();
}

#[cfg(test)]
mod tests {
    use super::*;
    use test::Bencher;

    #[test]
    fn test_is_prime_fast() {
        for i in 0..1000000 {
            let slow = is_prime(i);
            let fast = is_prime_fast(i);
            assert_eq!(
                slow,
                fast,
                "The number {} was unclear (slow:{}, fast:{})",
                i,
                slow,
                fast,
            );
        }
    }

    #[bench]
    fn bench_is_prime(b: &mut Bencher) {
        b.iter(|| {
            for i in 0..1000000 {
                is_prime(i);
            }
        });
    }

    #[bench]
    fn bench_is_prime_fast(b: &mut Bencher) {
        b.iter(|| {
            for i in 0..1000000 {
                is_prime_fast(i);
            }
        });
    }
}
