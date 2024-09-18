#pragma once

/*
题目：
一个回文数(palindromic number)指的是从左向右和从右向左读都一样的数字。最大的由两个两位数乘积构成的回文数是 9009 = 91 * 99。
找出最大的有由个三位数乘积构成的回文数。
*/

#include "basic_headers.h"

size_t reverse_ordered(size_t num) {
    if (num < 10) return num;

    vector<size_t> digits{};
    while (0 != num) {
        digits.push_back(num%10);
        num /= 10;
    }

    size_t reversed_num = 0;
    for (size_t digit : digits) {
        reversed_num = reversed_num * 10 + digit;
    }
    return reversed_num;
}

bool is_palindromic(size_t num) {
    if (0 == num) return true;
    if (0 == num%10) return false;
    return num == reverse_ordered(num);
}

bool can_be_mutiplied_by_two_3digit(size_t num) {
    if (num < 100*100) return false;
    if (num > 999*999) return false;
    for (size_t factor = 100; factor < 1000; ++factor) {
        if (0 != num % factor) continue;

        size_t devid = num/factor;
        if (devid < 100) return false;

        if (devid > 999) continue;

        cout << num << " = " << factor << " * " << devid << endl;
        return true;
    }
    return false;
}

void resolution() {
    const size_t max_number = 999*999;
    const size_t min_number = 100*100;

    for (size_t num = max_number; num >= min_number; --num) {
        if (is_palindromic(num) && can_be_mutiplied_by_two_3digit(num)) {
            cout << "result is " << num << endl;
            break;
        }
    }
}
