#pragma once

#include "basic_headers.h"

/*
题目：
10 以下的自然数中，属于 3 或 5 的倍数的有 3, 5, 6 和 9，它们之和是 23。
找出 1000 以下的自然数中，属于 3 或 5 的倍数的数字之和。
*/

void resolution() {
    size_t sum_3 = (3 + 999) * 333 / 2; //3+6+9+...+999
    size_t sum_5 = (5 + 995) * 199 / 2; //5+10+15+...+995
    size_t sum_15 = (15 + 990) * 66 / 2; //15+30+...+990
    std::cout << "result is " << sum_3 + sum_5 - sum_15 << std::endl;
}


