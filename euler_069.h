#ifndef __EULER_069_H__
#define __EULER_069_H__

#include "basic_headers.h"

void prepare(void** number_with_primes, int cnt) {
    for (int i = 1; i < cnt; ++i) {
        if (number_with_primes[i] == nullptr) {
            //k = i+1, k is a prime number,so 2*k ,3*k ... n*k are not prime.
            for (int j = 2 * i + 1; j < cnt; j += i + 1) {
                if (number_with_primes[j] == nullptr)
                    number_with_primes[j] = new vector<int>;
                ((vector<int>*)(number_with_primes[j]))->push_back(i + 1);
            }
        }
    }
}

int cnt_of_relatively_primes(void** number_with_primes, int cnt, int number) {
    if (number <= 0 || cnt < number) {
        return -1;
    }

    void* p_number = number_with_primes[number - 1];
    if (p_number == nullptr) {
        // number 本身是质数
        return number - 1;
    }

    const vector<int>& primes_of_number = *((vector<int>*)p_number);
    static char aux[1000000] = { 0 };
    memset(aux, 0, sizeof(aux));
    for (const int prime : primes_of_number) {
        for (int i = prime - 1; i < number; i += prime) {
            aux[i] = 'A';
        }
    }

    int n = 0;
    for (int i = 0; i < number - 1; ++i) {
        if (aux[i] == '\0') {
            ++n;
        }
    }
    return n;
}

void resolution() {

    void** number_with_primes = (void**)(new char[4000000]);
    memset(number_with_primes, 0, 1000000 * sizeof(void*));
    prepare(number_with_primes, 1000000);

    int special_n = 0;
    double ratio = 1.0;
    for (int number = 2; number <= 1000000; ++number) {
        if (number % 1000 == 0) {
            cout << "tip : current process " << number << endl;
        }
        int cnt = cnt_of_relatively_primes(number_with_primes, 1000000, number);
        double r = 1.0*number / cnt;
        if (r > ratio) {
            ratio = r;
            special_n = number;
            cout << "number " << number << " has " << cnt << " relatively primes. ratio = " << ratio << endl;
        }
    }

    cout << "At last, when n = " << special_n << ", ratio is " << ratio << " (the biggest value of ratio)." << endl;
}

//答案 510510 ，刚好是 2*3*5*7*11*13*17 ，这会是巧合吗？显然不是啊。
//
//原因：
//要找出所有n以内的与其互质的数的个数R，不如先找出所有不与之互质的数S，显然 R = n-S
//假设n的所有质因数有 P1 P2 P3 ... Pk k个，也就是说n可以被每一个质数Pi整除得到一个整数 n/Pi
//显然 (n,P1) (n,P1*2)  (n,P1*3) ...  (n,P2) (n,P2*2) ... (n,Pk) (n,Pk*2)... 每一组都不会是互质的
//上面的括号数对，第一个数都是n没啥可说的，第二个数如果是P1的倍数，分别从1倍的本身，到 n/P1 倍的n本身
//因此这样的数一共有 n/P1 + n/P2 + ... + n/Pk 个
//但是这会超过S很多，因为存在一些数，不仅是P1的倍数也是P2的倍数，这样的数就被统计了两次次数
//因此需要减掉重复计算的这样的数，那么就以同时是 Pi 和 Pj 的倍数为例，这样的数有多少个呢，很简单，有 n/(Pi*Pj) 个啊
//所以修正后 S = n/P1 + n/P2 + ... + n/Pk - n/(P1*P2) - n/(P1*P3) - ... - n/(P1*Pk) - n/(P2*P3) - n/(P2*P4) - ... - n/(Pk-1 * Pk)
//但是这样又错了，如果一个数同时是 P1 P2 P3 的倍数，那么它就被多次减掉，反而导致S算小了
//类推，同时是两个质数乘积的倍数的要减掉，同时是三个质数乘积的倍数的要加上，同时是四个质数乘积的倍数的再减掉...
//一直算到同时是 k个质数的乘积的倍数的要（加上/减掉）为止，才算是能得到S的正确公式
//这个过程还是很复杂的，但是至少是一个公式，理论上比起遍历每一个比n小的数之类的方式要更快（对计算机而言）
//一番数学计算之后 R = n - S 就得到了一个跟 n 以及 P1,P2...Pk 相关的公式：
//R = n * (1-1/P1) * (1-1/P2) * (1-1/P3) * ... (1-1/Pk)      //这一步是最重要的
//ratio = n/R = 1/[(1-1/P1) * (1-1/P2) * (1-1/P3) * ... (1-1/Pk)] 
//      = (P1/(P1-1)) * (P2/(P2-1)) * ... * (Pk/(Pk-1))
// 显然每一个 (P1/(P1-1)) 都是大于1的
// 为了ratio尽可能大，这样的质数 P1 P2 ... Pk越多越好了，也就是n分解出来的不同的质数尽可能多
// 那么显然最好n是 P1 * P2 * ... Pk (每个质数都是n的一次质数，不会有平方之类的），才能让n在尽量小的情况下，包含的不同的质数尽可能多
// 所以在1000000 以内的，就是用最小的那几个质数相乘，得到了 2*3*5*7*11*13*17 = 510510 

//有了这个公式和推理，代码写的算法就没有任何意义了，因为这个结果一行数字计算就得到了。



#endif /*__EULER_069_H__*/