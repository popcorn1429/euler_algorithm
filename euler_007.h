#pragma once
/*
题目：
前六个质数是 2, 3, 5, 7, 11 和 13，其中第 6 个是 13。
第 10001 个质数是多少？
*/

#include "basic_headers.h"

bool is_prime(uint64_t number, const list<uint64_t>& primes) {
    for (auto prime : primes) {
        if (0 == number % prime) return false;
        if (prime * prime > number) break;
    }
    return true;
}

void resolution() {
    list<uint64_t> primes{2,3,5,7};
    size_t count = 4;
    for (uint64_t num = 9; count < 10001; num += 2) {
        if (is_prime(num, primes)) {
            primes.push_back(num);
            ++count;
        }
    }

    cout << "result is " << *primes.rbegin() << endl;
}
