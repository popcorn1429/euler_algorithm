#pragma once

#include "basic_headers.h"

/*
题目：
斐波那契数列中的每一项被定义为前两项之和。从 1 和 2 开始，斐波那契数列的前十项为：
1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ...
考虑斐波那契数列中数值不超过 4 百万的项，找出这些项中值为偶数的项之和。
*/

size_t next_fibonacci(size_t (&fibonacci)[3], size_t& pos) {
    fibonacci[pos] = fibonacci[(pos + 1)%3] + fibonacci[(pos + 2)%3];
    size_t next = fibonacci[pos];
    pos = (pos + 1)%3;
    return next;
}

void resolution() {
    size_t fibonacci[3]{0,1,0};
    size_t next_pos = 2;

    size_t sum = 0;
    size_t next = next_fibonacci(fibonacci, next_pos);
    while (next <= 4000000) {
        std::cout << "next is " << next << std::endl;
        if (next % 2 == 0)
            sum += next;
        next = next_fibonacci(fibonacci, next_pos);
    }

    std::cout << "result is " << sum << std::endl;
}