/** A simple C++ program to serve as a benchmark for assessing the
    performance difference between std::vector::at() method versus
    std::vector::operator[] to access individual elements in the vector.

    Copyright (C) raodm@miamiOH.edu
*/

#include <vector>
#include <iostream>

int
sum_at(const std::vector<int>& vec) {
    int sum = 0;
    for (size_t i = 0; (i < vec.size()); i++) {
        sum += vec.at(i);
    }
    return sum;
}

int
sum(const std::vector<int>& vec) {
    int sum = 0;
    for (size_t i = 0; (i < vec.size()); i++) {
        sum += vec[i];
    }
    return sum;
}


int
main() {
    // NOTE: Keep vector to 1 MB to ensure optimal use of cache
    std::vector<int> vec(1000000, 1);
    long total = 0, sign = 1;
    // NOTE: The number of iterations is set so that the benchmark
    // runs for reasonable time to obtain reliable runtime measurements
    for (int testCount = 0; (testCount < 2000); testCount++) {
        // total += sum(vec) * sign;
        total += sum_at(vec) * sign;
        sign  *= -1;
    }
    std::cout << "Total from array test = " << total << std::endl;
    return 0;
}
