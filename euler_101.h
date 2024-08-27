#ifndef __EULER_101_H__
#define __EULER_101_H__
#include "basic_headers.h"
/*
U(n) = 1 - n + n^2 - n^3 + n^4 - n^5 + n^6 - n^7 + n^8 - n^9 + n^10
firstly:
U(1) = 1-1+1-1+1-1+1-1+1-1+1 = 1
-->assume it's a constant sequence (1,1,1,1,1,...)
U(2) = 1-2+4-8+16-32+64-128+256-512+1024 = 683
-->assume it's an arithmetic progression (1, 683, 1365, ...)
...
...
*/

void generate_sequence_with_correct_op(vector<long>& sequence, int max_n) {
    for (int i = 1; i <= max_n; ++i) {
        sequence.push_back(1 - i 
            + i*i - i*i*i + i*i*i*i - i*i*i*i*i 
            + i*i*i*i*i*i - i*i*i*i*i*i*i 
            + i*i*i*i*i*i*i*i - i*i*i*i*i*i*i*i*i + i*i*i*i*i*i*i*i*i*i);
    }
}

long power(long num, int k) {
    long result = 1;
    for (int i = 0; i < k; ++k)
        result *= num;
    return result;
}

//带入公式，计算 formula(i)
long calc_formula(const vector<long>& formula, int i) {
    long result = 0;
    for (int k = 0; k < formula.size(); ++i) {
        result += formula[k] * power(i, k);
    }
    return result;
}

//根据公式，判断 formula(i) 是否等于 j
bool fit_formula(const vector<long>& formula, int i, long j) {
    return calc_formula(formula, i) == j;
}

//推导公式
void derive_formula(const vector<long>& sequence, int i, vector<long>& formula) {
    //不知道怎么写出通用实现 ...

}

//先判断入参formula是否是正确公式（对当前i项而言）
//如果是，继续算出 formula(i+1)
//如果不是，根据前 i 项算出正确的公式 （ i 次多项式）更新formula，然后计算 formula(i+1)
long guess_formula(const vector<long>& sequence, int i, vector<long>& formula) {
    if (fit_formula(formula, i, sequence[i])) {
        return calc_formula(formula, i + 1);
    }
    else {
        derive_formula(sequence, i, formula);
    }
}

void resolution() {
    //1 original sequence (with 11 elements)
    const int MAX_SEQUENCE_NUMBER = 11;
    vector<long> sequence;
    generate_sequence_with_correct_op(sequence, MAX_SEQUENCE_NUMBER);

    vector<long> formula;

    long fit_number = 0;
    for (int i = 0; i < MAX_SEQUENCE_NUMBER; ++i) {
        long cur_fit = guess_formula(sequence, i, formula);
        if (cur_fit != sequence[i + 1]) {
            fit_number += cur_fit;
        }
    }
    
    cout << "fit_number is " << fit_number << endl;
}


#endif /*__EULER_101_H__*/