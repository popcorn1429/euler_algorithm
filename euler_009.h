#pragma once

/*
题目：
一个毕达哥拉斯三元组(Pythagorean triplet)是一个包含三个自然数的集合，a < b < c，满足条件：
    a^2 + b^2 = c^2

例如：3^2 + 4^2 = 9 + 16 = 25 = 5^2。

已知存在并且只存在一个毕达哥拉斯三元组满足条件 a + b + c = 1000。
找出该三元组中 abc 的乘积。
*/

#include "basic_headers.h"

using multiplied = size_t;

void resolution() {
    //c^2 = a^2 + b^2 < (a + b)^2
    //so, c < a + b
    //1000 = a + b + c < 2(a+b)
    //so a+b > 500, c < 500
    //and obviously, c > a, c > b, so c > 333
    //a^2 + b ^2 = (1000 - a - b)^2
    // = 1000000 + a^2 + b^2 - 2000a - 2000b + 2ab
    //-> 0 = 1000000 - 2000a - 2000b + 2ab
    //1000000 = 2000a + 2000b - 2*a*b
    
    //667 > a + b > 500, and 1000000 = 2000a + 2000b - 2*a*b
    //if a < b, then 1 < a < 333

    for (size_t a = 1; a < 333; ++a) {
        for (size_t b = 500 - a; a + b < 667; ++b) {
            if (2000 * a + 2000 * b - 2 * a * b == 1000000) {
                cout << "get a = " << a << ", b = " << b << ", c = " << (1000 - a - b) << endl;
                cout << "result is " << a * b * (1000 - a - b) << endl;
                return;
            }
        }
    }
    cout << "no result!" << endl;
}