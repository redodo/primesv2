#![feature(test)]

extern crate test;

use pbr::ProgressBar;
use std::collections::HashSet;

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

fn is_prime_fast(n: u64) -> bool {
    // A number divisible by 2 is only prime when it is 2.
    if n & 1 == 0 {
        return n == 2;
    }
    // The number 3 is prime.
    if n == 3 {
        return true;
    }
    // Numbers below 2 are not prime.
    if n < 2 {
        return false;
    }
    // Prime numbers larger than 3 are a result of 6n±1.
    //
    // If neither number beside the prime number is divisible by 6
    // it is not a prime number.
    // 
    // Gives a ~5% speed boost.
    let prev_mod = (n - 1) % 6;
    if prev_mod != 0 && prev_mod != 4 {
        return false;
    }
    // Check the number against a precomputed list of prime numbers.
    for (d, dxd) in INITIAL_DIVIDERS.iter() {
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

fn main() {
    let max = 1000000000;
    let step_update = 0xFFFF;
    let mut bar = ProgressBar::new(max);
    let mut divi: HashSet<u64> = HashSet::new();

    for i in 0..max {

        if i & step_update == step_update {
            bar.add(step_update);
        }

        if !is_reduce_div(i-1) && !is_reduce_div(i+1) {
            for y in 2..10 {
                if i % y == 0 {
                    divi.insert(y);
                    break;
                }
            }
        }

        /*
        if is_prime(i) {
            if !is_reduce_div(i-1) && !is_reduce_div(i+1) {
                println!("{} failed", i);
                break;
            }
        }
        */
    }
    println!("{:?}", divi);
    bar.finish_print("All numbers were checked.");
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
            for i in 0..100000 {
                is_prime(i);
            }
        });
    }

    #[bench]
    fn bench_is_prime_fast(b: &mut Bencher) {
        b.iter(|| {
            for i in 0..100000 {
                is_prime_fast(i);
            }
        });
    }
}
