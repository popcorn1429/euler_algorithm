#pragma once

/*
题目：
13195 的质数因子有 5, 7, 13 和 29。
600851475143 的最大质数因子是多少？
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
    uint64_t target = 600851475143;
    for (uint64_t num = 9; num * num <= target; num += 2) {
        if (is_prime(num, primes)) {
            primes.push_back(num);
        }
    }

    for (auto iter = primes.rbegin(); iter != primes.rend(); ++iter) {
        if (0 == target % *iter) {
            cout << "result is " << *iter << endl;
            break;
        }
    }
}
