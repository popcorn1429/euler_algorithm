#ifndef __EULER_618_H__
#define __EULER_618_H__
#include "basic_headers.h"

/*

*/

const int BITS_OF_BYTE = 8;
const long BILLION = 1000000000;

template <typename Container>
void print_array(Container c, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        cout << c[i] << ' ';
    }
    cout << endl;
}

class SplitSet {
public:
    static int fibonacci[24];         //����ǰ24��쳲���������
    static vector<int> primes;        //������������С����
    static char bytes[8192];          //ɸѡ������Ѹ���ҳ�������Ҳ���ڿ����ж�һ�����Ƿ�������
public:
    //һ��ʼҪ�����������������
    static void calc_fibonacci() {
        fibonacci[0] = 1;
        fibonacci[1] = 1;
        for (int i = 2; i < 24; ++i) {
            fibonacci[i] = fibonacci[i - 2] + fibonacci[i - 1];
        }

        cout << "fibonacci : " << endl;
        print_array(fibonacci, 24);
    }
    //һ��ʼҪ�����������������
    static void calc_prime() {
        memset(bytes, 0, sizeof(bytes));
        int n = sizeof(bytes);
        int index = 0;
        int pos = 0;
        for (int i = 2; i < n * BITS_OF_BYTE; ++i) {
            index = i / BITS_OF_BYTE;
            pos = i % BITS_OF_BYTE;
            if ((bytes[index] & (0x01 << pos)) == 0) {
                primes.push_back(i);
                for (int j = i * 2; j < n * BITS_OF_BYTE; j += i) {
                    index = j / BITS_OF_BYTE;
                    pos = j % BITS_OF_BYTE;
                    bytes[index] |= (0x01 << pos);
                }
            }
        }

        cout << "primes (first 30) : " << endl;
        print_array(primes, 30);
    }

public:
    int number;
    vector<vector<int>> splits;//���еĲ�ַ�ʽ�ļ���
    vector<long>        products; //�����ÿһ����Ϸ�ʽ�ĳ˻�����������������ȫ��ͬ��λ��һһ��Ӧ����ÿ����ֻ�������9λ
    
    void reset(int n) {
        number = n;
        splits.clear();
        products.clear();
    }
public:
    
    //һ�������Ĳ�ֹ��̣�Ч�ʻ���number�ϴ�ʱ���Ա��
    //��ǰnumber��С��ǰ��û��������numberʱ����
    //�����ܵ�ʹ����һ�����غ����ķ�ʽ
    void split_and_product() {
        vector<int> factors;
        split(number, 0, factors);

        //print_and_check
        print_and_check();
    }
    //�����ǰ��number��ǰ��������С��number�ĺͣ���������С��number�Ѿ������Լ��������Ĳ�ּ�����
    //��ǰ�Ĳ�ֹ��̿��Խ���֮ǰ����number�Ĳ�����ݣ������ظ��Ͷ�
    void split_and_product(const SplitSet& left, const SplitSet& right) {
        clock_t t_step_1 = clock();
        splits.reserve(left.splits.size() * right.splits.size() * 2);
        //����1 �� �����ǰ��ż����ǰ����һ����������һ��������� 2+2+2+...+2 ��������ϣ����Ҳһ����Ҫ���ڣ������Ȳ���
        if (number % 2 == 0) {
            int n = number / 2;
            vector<int> m(n, 2);
            products.push_back(product_with_limit(m));
            splits.push_back(vector<int>());
            splits.back().swap(m);
        }
        clock_t t_step_2 = clock();
        //2 ǰ���������в����ϵ�������ϣ����ܳ����ظ��Ľ�ϣ���Ҫ�ж����أ�
        // ע�⣬ÿһ�������ʽ��������Ԫ�ص��������ں������ؼ�����������
        
        // ��һ����ʱ���ˣ�������number����������������
        // splits��ּ���Ѹ�����ͣ��ڴ�ķ�Ҳ��Ϊ�޴�
        // ��Ȼ���������㷨����ȷ�ģ����Ƕ��ڱ�������û�а�������ȫ�����ڿ����ܵ�ʱ�䷶Χ�����

        // ����ȥ��splits��������������ϸ��
        // ���ǻ�û���û��splitsҪ�����ȷ�õ�number��ֳ˻�������Ҫ������α�֤���в�ַ�����û���ظ�����©
        // ����⣬��Ԥ��Ҫ��β��~~
        size_t sz_left = left.products.size();
        size_t sz_right = right.products.size();
        vector<int> merged;
        long merge_time_span = 0;
        long merge_exist_span = 0;
        for (size_t l = 0; l < sz_left; ++l) {
            for (size_t r = 0; r < sz_right; ++r) {
                merge_time_span += merge(left.splits[l], right.splits[r], merged);
                if (merged_exists(merged, merge_exist_span)) {
                    merged.clear();
                    continue;
                }

                products.push_back(product_with_limit(merged));
                splits.push_back(vector<int>());
                splits.back().swap(merged);   //more efficient
            }
        }
        clock_t t_step_3 = clock();
        //3 �������
        int max_prime_in_left = max_prime(left.number);
        int max_prime_in_right = max_prime(right.number);
        int max_prime_before = (max_prime_in_left < max_prime_in_right) ? max_prime_in_right : max_prime_in_left;
        int max_prime_in_this = max_prime(number);
        size_t pos = 0;
        for (; pos < primes.size(); ++pos) {
            if (primes[pos] == max_prime_before) {
                break;
            }
        }
        ++pos;
        while (primes[pos] <= max_prime_in_this) {
            //�����Ǹ��ְ����� primes[pos] �Ĳ�ַ�ʽ������
            vector<int> factors;
            split(number - primes[pos], 0, factors, primes[pos]);  //��������о޴���������⣨��Ȼnumber����С�ˣ�������Ȼ��һ������Ĳ�֣���֮ǰ�ܿ�������ͬ������ֵ�Ĳ�֣�
            ++pos;
        }
        clock_t t_step_4 = clock();
        cout << "time span , step1:" << t_step_2 - t_step_1
            << ", step2:" << t_step_3 - t_step_2
            << ", merge time span:" << merge_time_span 
            << ", merge exist span:" << merge_exist_span
            << ", step3:" << t_step_4 - t_step_3 << endl;
        print_and_check();
    }


    //��n��ֳ�����֮�ͣ����ֿ��ܵ����ȫ����������cur_prime ���������ʱ��Ҫ���ϵĶ���һ������
    void split(int n, size_t pos, vector<int>& factors, int cur_prime) {
        if (n == 0) {
            //product all elements of factors
            long t = product_with_limit(factors);
            products.push_back(product_with_limit(t, cur_prime));
            splits.push_back(vector<int>());
            merge(factors, cur_prime, splits.back());
        }

        for (size_t i = pos; i < primes.size(); ++i) {
            if (primes[i] <= n) {
                factors.push_back(primes[i]);
                split(n - primes[i], i, factors, cur_prime);
                factors.pop_back();
            }
            else {
                break;
            }
        }
    }
    //��n��ֳ�����֮�ͣ����ֿ��ܵ����ȫ��������
    void split(int n, size_t pos, vector<int>& factors) {
        if (n == 0) {
            //product all elements of factors
            products.push_back(product_with_limit(factors));
            splits.push_back(factors);
        }

        for (size_t i = pos; i < primes.size(); ++i) {
            if (primes[i] <= n) {
                factors.push_back(primes[i]);
                split(n - primes[i], i, factors);
                factors.pop_back();
            }
            else {
                break;
            }
        }
    }

private:
    //�жϵ�ǰ���еĲ�ַ�����Ƿ��Ѵ�����merged��ʽ��ͬ����
    bool merged_exists(const vector<int>& merged, long& time_span) {
        clock_t t_merge_exist_begin = clock();
        for (const vector<int>& v : splits) {
            if (is_same(v, merged)) {
                clock_t t_merge_exist_end = clock();
                time_span += t_merge_exist_end - t_merge_exist_begin;
                return true;
            }
        }
        clock_t t_merge_exist_end = clock();
        time_span += t_merge_exist_end - t_merge_exist_begin;
        return false;
    }
    //�ж������������Ƿ���ͬ��ÿ�������о���һ����ַ�����������������ģ�
    bool is_same(const vector<int>& v, const vector<int>& merged) const {
        if (v.size() != merged.size())
            return false;

        for (size_t i = v.size()-1; i != 0; --i) {
            if (v[i] != merged[i])
                return false;
        }
        return true;
    }
    
private:
    //�ϲ����������У��������򣺴�С����
    long merge(const vector<int>& vl, const vector<int>& vr, vector<int>& merged) const {
        clock_t t_merge_begin = clock();
        merged.reserve(vl.size() + vr.size());
        size_t l = 0;
        size_t r = 0;
        while (l < vl.size() && r < vr.size()) {
            if (vl[l] < vr[r]) {
                merged.push_back(vl[l]);
                ++l;
            }
            else {
                merged.push_back(vr[r]);
                ++r;
            }
        }

        while (l < vl.size()) {
            merged.push_back(vl[l]);
            ++l;
        }
        while (r < vr.size()) {
            merged.push_back(vr[r]);
            ++r;
        }
        clock_t t_merge_end = clock();
        //cout << "merge time : " << t_merge_end - t_merge_begin << endl;
        return t_merge_end - t_merge_begin;
    }
    //�ϲ����к�һ�����֣��������򣺴�С����
    void merge(const vector<int>& v, const int n, vector<int>& merged) {
        merged.reserve(v.size() + 1);
        size_t i = 0;
        for (; i < v.size(); ++i) {
            if (v[i] < n) {
                merged.push_back(v[i]);
            }
            else {
                break;
            }
        }

        merged.push_back(n);
        for (; i < v.size(); ++i) {
            merged.push_back(v[i]);
        }
    }

private:
    //�����ж�
    bool is_prime(int number) {
        if (BITS_OF_BYTE*sizeof(bytes) <= number) {
            return false;
        }

        int index = number / BITS_OF_BYTE;
        int pos = number % BITS_OF_BYTE;
        return 0x00 == (bytes[index] & (0x01 << pos));
    }
    //�ҳ�С�ڵ���n����������������n-1������һ����Сһ���
    int max_prime(int n) {
        //���n���������򷵻�n
        if (is_prime(n))
            return n;

        //���򣬻�ȡ��nС���������m������m = n-1ʱ���ұ�mС�������������Ϊn�����Բ�ֳ� m + 1 ��������ϣ�
        for (size_t i = 0; i < primes.size(); ++i) {
            if (n < primes[i]) {
                if (0 < i) {
                    size_t j = i - 1;
                    if (n - primes[j] == 1) {
                        if (0 < j) {
                            return primes[j - 1];
                        }
                    }
                    else {
                        return primes[j];
                    }
                }

                break; //ǰ���return ���ǳ��ڣ�Ӧ�ò�������Ҫ�ߵ����break����ֻ�����﷨�ϵ�һ������
            }
        }

        return 0; //����Ҳ�ǲ�Ӧ���ߵ�����䣬����n������������
    }

public:
    //�������е�����Ԫ���۳� ������˻�����9λ�����������ĩβ��9λ��
    long product_with_limit(const vector<int>& factors) {
        long t = 1;
        for (int n : factors) {
            t = product_with_limit(t, n);
        }
        return t;
    }
    //����������ˣ�����˻���һ������9λ���������������ĩβ��9λ��
    long product_with_limit(long first, long second) {
        if (BILLION <= first || BILLION <= second) {
            cout << "ERROR : product_with_limit (" << first << ", " << second << ")." << endl;
            return -1; //error!
        }

        long temp_result = first * second;
        return temp_result % BILLION;
    }
    //����������ӣ�����ͳ���9λ���������ĩβ9λ��
    long plus_with_limit(long first, long second) {
        if (BILLION <= first || BILLION <= second) {
            cout << "ERROR : plus_with_limit (" << first << ", " << second << ")." << endl;
            return -1; //error!
        }

        long temp_result = first + second;
        return temp_result % BILLION;
    }
   
private:
    //��ӡ�õģ�û����������
    void print_and_check() const {
        cout << "number :" << number << ", cnt of splits : " << splits.size() << endl;
        /*
        cout << "number is " << number << endl;
        for (size_t i = 0; i < products.size(); ++i) {
            const vector<int>& v = splits[i];
            cout << " ";
            for (int n : v) {
                cout << n << ' ';
            }
            cout << " \n---> " << products[i] << endl;
        }

        cout << "----------------------------" << endl;
        */
    }
};

int SplitSet::fibonacci[24] = { 0 };
vector<int> SplitSet::primes = vector<int>();
char SplitSet::bytes[8192] = { 0 };

void resolution() {
    SplitSet::calc_fibonacci();
    SplitSet::calc_prime();
    SplitSet sets[3];
    for (int i = 2; i < 13; ++i) {
        clock_t t_begin = clock();
        cout << "fibonacci[" << i << "] is " << SplitSet::fibonacci[i] << endl;
        if (i == 2) {
            sets[0].reset(SplitSet::fibonacci[i]);
            sets[0].split_and_product();
        }
        else if (i == 3) {
            sets[1].reset(SplitSet::fibonacci[i]);
            sets[1].split_and_product();
        }
        else {
            sets[(i+1) % 3].reset(SplitSet::fibonacci[i]);
            sets[(i+1) % 3].split_and_product(sets[(i + 2) % 3], sets[(i + 3) % 3]);
        }
        clock_t t_end = clock();
        cout << "calc S(" << SplitSet::fibonacci[i] << ") ,costs " << 1000 * (t_end - t_begin) / CLOCKS_PER_SEC << " ms.\n" << endl;
    }

//    SplitSet _set;

}


#endif /*__EULER_618_H__*/