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
    static int fibonacci[24];         //保存前24跟斐波那契数列
    static vector<int> primes;        //保存质数（从小到大）
    static char bytes[8192];          //筛选法用于迅速找出质数，也用于快速判断一个数是否是质数
public:
    //一开始要靠这个函数生成数列
    static void calc_fibonacci() {
        fibonacci[0] = 1;
        fibonacci[1] = 1;
        for (int i = 2; i < 24; ++i) {
            fibonacci[i] = fibonacci[i - 2] + fibonacci[i - 1];
        }

        cout << "fibonacci : " << endl;
        print_array(fibonacci, 24);
    }
    //一开始要靠这个函数生成质数
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
    vector<vector<int>> splits;//所有的拆分方式的集合
    vector<long>        products; //上面的每一种组合方式的乘积（数据量跟上面完全相同且位置一一对应），每个数只留下最后9位
    
    void reset(int n) {
        number = n;
        splits.clear();
        products.clear();
    }
public:
    
    //一个基础的拆分过程，效率会在number较大时明显变差
    //当前number较小，前面没有其他的number时可用
    //尽可能的使用下一个重载函数的方式
    void split_and_product() {
        vector<int> factors;
        split(number, 0, factors);

        //print_and_check
        print_and_check();
    }
    //如果当前的number是前面两个更小的number的和，那两个更小的number已经有了自己的完整的拆分集合了
    //当前的拆分过程可以借助之前两个number的拆分数据，避免重复劳动
    void split_and_product(const SplitSet& left, const SplitSet& right) {
        clock_t t_step_1 = clock();
        splits.reserve(left.splits.size() * right.splits.size() * 2);
        //规律1 ： 如果当前是偶数，前面俩一定是奇数，一定不会带来 2+2+2+...+2 这样的组合，这个也一定需要存在，所以先补上
        if (number % 2 == 0) {
            int n = number / 2;
            vector<int> m(n, 2);
            products.push_back(product_with_limit(m));
            splits.push_back(vector<int>());
            splits.back().swap(m);
        }
        clock_t t_step_2 = clock();
        //2 前两个数所有拆解组合的两两结合（可能出现重复的结合，需要判断排重）
        // 注意，每一个拆分形式，都保持元素递增（便于后面排重及其他操作）
        
        // 这一步耗时惊人，且随着number的增大，其增长极快
        // splits拆分集合迅速膨胀，内存耗费也极为巨大
        // 虽然可能这套算法是正确的，但是对于本题的求解没有帮助，完全不能在可忍受的时间范围内完成

        // 考虑去掉splits容器，不保存拆分细节
        // 但是还没想好没了splits要如何正确得到number拆分乘积，最重要的是如何保证所有拆分方案都没有重复和遗漏
        // 这道题，我预感要烂尾了~~
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
        //3 其他情况
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
            //这里是各种包含了 primes[pos] 的拆分方式的生成
            vector<int> factors;
            split(number - primes[pos], 0, factors, primes[pos]);  //这里可能有巨大的性能问题（虽然number被减小了，但是依然是一个琐碎的拆分，且之前很可能做过同样的数值的拆分）
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


    //将n拆分成质数之和（各种可能的组合全部处理到），cur_prime 是最后计算的时候要带上的额外一项质数
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
    //将n拆分成质数之和（各种可能的组合全部处理到）
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
    //判断当前已有的拆分方案里，是否已存在与merged方式相同的了
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
    //判断两个数据列是否相同（每个数据列就是一个拆分方案，都是数据有序的）
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
    //合并两个数据列（保持有序：从小到大）
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
    //合并数列和一个数字（保持有序：从小到大）
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
    //质数判断
    bool is_prime(int number) {
        if (BITS_OF_BYTE*sizeof(bytes) <= number) {
            return false;
        }

        int index = number / BITS_OF_BYTE;
        int pos = number % BITS_OF_BYTE;
        return 0x00 == (bytes[index] & (0x01 << pos));
    }
    //找出小于等于n的最大质数，如果是n-1，则找一个更小一点的
    int max_prime(int n) {
        //如果n是质数，则返回n
        if (is_prime(n))
            return n;

        //否则，获取比n小的最大质数m，仅当m = n-1时，找比m小的最大质数（因为n不可以拆分成 m + 1 这样的组合）
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

                break; //前面的return 才是出口，应该不可能需要走到这个break，这只是在语法上的一个保险
            }
        }

        return 0; //这里也是不应该走到的语句，除非n本身不是正整数
    }

public:
    //将数据列的所有元素累乘 （如果乘积超过9位数，则仅保留末尾的9位）
    long product_with_limit(const vector<int>& factors) {
        long t = 1;
        for (int n : factors) {
            t = product_with_limit(t, n);
        }
        return t;
    }
    //将两个数相乘（如果乘积是一个超过9位数的数，则仅保留末尾的9位）
    long product_with_limit(long first, long second) {
        if (BILLION <= first || BILLION <= second) {
            cout << "ERROR : product_with_limit (" << first << ", " << second << ")." << endl;
            return -1; //error!
        }

        long temp_result = first * second;
        return temp_result % BILLION;
    }
    //将两个数相加（如果和超过9位，则仅保留末尾9位）
    long plus_with_limit(long first, long second) {
        if (BILLION <= first || BILLION <= second) {
            cout << "ERROR : plus_with_limit (" << first << ", " << second << ")." << endl;
            return -1; //error!
        }

        long temp_result = first + second;
        return temp_result % BILLION;
    }
   
private:
    //打印用的，没有其他作用
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