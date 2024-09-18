#pragma once

/*
题目：
10 以下的质数的和是 2 + 3 + 5 + 7 = 17。
找出两百万以下所有质数的和。
*/

#include "basic_headers.h"

bool is_prime(uint64_t number, const list<uint64_t>& primes) {
    for (auto prime : primes) {
        if (0 == number % prime) return false;
        if (prime * prime > number) break;
    }
    return true;
}


using summary = uint64_t;

void resolution() {
    list<uint64_t> primes{2,3,5,7};
    summary sum = 2 + 3 + 5 + 7;
    for (uint64_t num = 9; num < 2000000; num += 2) {
        if (is_prime(num, primes)) {
            primes.push_back(num);
            sum += num;
        }
    }
    
    cout << "result is " << sum << endl;
}
