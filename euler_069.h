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
        // number ����������
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

//�� 510510 ���պ��� 2*3*5*7*11*13*17 ��������ɺ�����Ȼ���ǰ���
//
//ԭ��
//Ҫ�ҳ�����n���ڵ����以�ʵ����ĸ���R���������ҳ����в���֮���ʵ���S����Ȼ R = n-S
//����n�������������� P1 P2 P3 ... Pk k����Ҳ����˵n���Ա�ÿһ������Pi�����õ�һ������ n/Pi
//��Ȼ (n,P1) (n,P1*2)  (n,P1*3) ...  (n,P2) (n,P2*2) ... (n,Pk) (n,Pk*2)... ÿһ�鶼�����ǻ��ʵ�
//������������ԣ���һ��������nûɶ��˵�ģ��ڶ����������P1�ı������ֱ��1���ı����� n/P1 ����n����
//�����������һ���� n/P1 + n/P2 + ... + n/Pk ��
//������ᳬ��S�ܶ࣬��Ϊ����һЩ����������P1�ı���Ҳ��P2�ı��������������ͱ�ͳ�������δ���
//�����Ҫ�����ظ������������������ô����ͬʱ�� Pi �� Pj �ı���Ϊ�������������ж��ٸ��أ��ܼ򵥣��� n/(Pi*Pj) ����
//���������� S = n/P1 + n/P2 + ... + n/Pk - n/(P1*P2) - n/(P1*P3) - ... - n/(P1*Pk) - n/(P2*P3) - n/(P2*P4) - ... - n/(Pk-1 * Pk)
//���������ִ��ˣ����һ����ͬʱ�� P1 P2 P3 �ı�������ô���ͱ���μ�������������S��С��
//���ƣ�ͬʱ�����������˻��ı�����Ҫ������ͬʱ�����������˻��ı�����Ҫ���ϣ�ͬʱ���ĸ������˻��ı������ټ���...
//һֱ�㵽ͬʱ�� k�������ĳ˻��ı�����Ҫ������/������Ϊֹ���������ܵõ�S����ȷ��ʽ
//������̻��Ǻܸ��ӵģ�����������һ����ʽ�������ϱ������ÿһ����nС����֮��ķ�ʽҪ���죨�Լ�������ԣ�
//һ����ѧ����֮�� R = n - S �͵õ���һ���� n �Լ� P1,P2...Pk ��صĹ�ʽ��
//R = n * (1-1/P1) * (1-1/P2) * (1-1/P3) * ... (1-1/Pk)      //��һ��������Ҫ��
//ratio = n/R = 1/[(1-1/P1) * (1-1/P2) * (1-1/P3) * ... (1-1/Pk)] 
//      = (P1/(P1-1)) * (P2/(P2-1)) * ... * (Pk/(Pk-1))
// ��Ȼÿһ�� (P1/(P1-1)) ���Ǵ���1��
// Ϊ��ratio�����ܴ����������� P1 P2 ... PkԽ��Խ���ˣ�Ҳ����n�ֽ�����Ĳ�ͬ�����������ܶ�
// ��ô��Ȼ���n�� P1 * P2 * ... Pk (ÿ����������n��һ��������������ƽ��֮��ģ���������n�ھ���С������£������Ĳ�ͬ�����������ܶ�
// ������1000000 ���ڵģ���������С���Ǽ���������ˣ��õ��� 2*3*5*7*11*13*17 = 510510 

//���������ʽ����������д���㷨��û���κ������ˣ���Ϊ������һ�����ּ���͵õ��ˡ�



#endif /*__EULER_069_H__*/