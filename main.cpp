#include "euler_011.h"
#include <chrono>

int main() {

    const auto start = std::chrono::steady_clock::now();

    resolution();

    const auto end = std::chrono::steady_clock::now();

    std::chrono::nanoseconds dur = end - start;

    std::cout << "resolution took "
        << (end - start).count() << " nanoseconds = "
        << (end - start).count()/1000000.0 << " ms" << std::endl;
    system("pause");
    return 0;
}
