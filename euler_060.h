#ifndef __EULER_060_H__
#define __EULER_060_H__

#include "basic_headers.h"

void prepare_primes(unsigned char* primes, size_t bytes) {
    const size_t BITS_PER_BYTE = 8;
    for (size_t i = 2; i < bytes * BITS_PER_BYTE; ++i) {
        size_t byte_pos = i / BITS_PER_BYTE;
        size_t bit_pos = i % BITS_PER_BYTE;
        if ((primes[byte_pos] & (0x01 << bit_pos)) == 0) {
            //this is a prime
            //cout << "get a prime " << i << endl;
            for (size_t j = i * 2; j < bytes * BITS_PER_BYTE; j += i) {
                byte_pos = j / BITS_PER_BYTE;
                bit_pos = j % BITS_PER_BYTE;
                primes[byte_pos] |= (0x01 << bit_pos);
            }
        }
    }
}

void print_primes_bytes(const unsigned char* primes, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X ", primes[i]);
    }
    printf("\n");
}

bool is_prime(const unsigned char* primes, size_t bytes, int number) {
    const size_t BITS_PER_BYTE = 8;
    size_t byte_pos = number / BITS_PER_BYTE;
    if (bytes <= byte_pos) {
        cout << "[is_prime] number " << number << " is too large!" << endl;
        return false;
    }
    size_t bit_pos = number % BITS_PER_BYTE;

    return ((unsigned char)(0x01 << bit_pos) & primes[byte_pos]) == '\0';
}

int concatenate(int left, int right) {
    int ratio = 1;
    while (ratio <= right) {
        ratio *= 10;
    }

    return left * ratio + right;
}

bool are_all_primes(const unsigned char* primes, size_t bytes, const vector<int>& special_primes, int number) {
    for (int prime : special_primes) {
        int new_number = concatenate(prime, number);
        if (new_number == -1 || !is_prime(primes, bytes, new_number)) {
            //cout << "concatenated number " << new_number << " (by " << prime << "+" << number << ") is not prime!" << endl;
            return false;
        }

        new_number = concatenate(number, prime);
        if (new_number == -1 || !is_prime(primes, bytes, new_number)) {
            //cout << "concatenated number " << new_number << " (by " << prime << "+" << number << ") is not prime!" << endl;
            return false;
        }
    }
    return true;
}


void resolution() {
    vector<int> special_primes;
    special_primes.push_back(3);
    special_primes.push_back(7);
    special_primes.push_back(109);
    special_primes.push_back(673);

    const size_t BITS_PER_BYTE = 8;
    const size_t MAX_BYTES = 1024 * 1024 * 1024; //无法申请这么大的空间 说明这道题不能这么解
    unsigned char* primes = nullptr;
    try {
        primes = new unsigned char[MAX_BYTES];
    }
    catch (...)
    {
        cout << "exception !" << endl;
        return;
    }
    memset(primes, 0, MAX_BYTES);
    clock_t t_begin = clock();
    prepare_primes(primes, MAX_BYTES);
    clock_t t_end = clock();
    cout << " prepare_primes costs " << 1000 * (t_end - t_begin) / CLOCKS_PER_SEC << endl;

    long limit_max_number = BITS_PER_BYTE*MAX_BYTES / 1024;
    cout << "limit is " << limit_max_number << endl;
    for (int number = 677; number < limit_max_number; number += 2) {
        if (is_prime(primes, MAX_BYTES, number)) {  //首先number本身是一个质数
            if (are_all_primes(primes, MAX_BYTES, special_primes, number)) { //其次number与前几个质数的任意拼接都是一个新质数
                cout << "when number is " << number << ", all can be primes!" << endl;
                break;
            }
        }
    }
}


#endif /*__EULER_060_H__*/