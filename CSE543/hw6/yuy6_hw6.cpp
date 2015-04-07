/**
 *  Copyright [2015] <Yan Yu>
 */
#include <omp.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "PropertiesChecker.h"

std::vector<numProperties>
constructNumProperties(const long long& lo, const long long& hi) {
    std::vector<numProperties> elements(hi - lo + 1);
    long long i = lo;
    for_each(elements.begin(), elements.end(),
               [&i](numProperties& e) {
                   e._num = i++;
               });
    return elements;
}

void
printResult(const std::vector<numProperties>& elements) {
    auto print = [](const numProperties& e) {
        std::cout << e._num << " "
                  << (e._isPrime ? "is" : "not") << " prime, "
                  << (e._isPrimeFactorable ? "is" : "not") << " factorable, "
                  << (e._isFactorial ? "is" : "not") << " factorial, "
                  << (e._isPalindromic ? "is" : "not") << " pallindrome\n";
    };
    for_each(elements.begin(), elements.end(), print);
}

int
main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Input Format: <./Your Program> "
                  << "<Lower Bound> <Upper Bound>\n";
        return 1;
    }

    long long lo = std::stoll(argv[1]), hi = std::stoll(argv[2]);
    std::vector<numProperties> numProperties = constructNumProperties(lo, hi);

    #pragma omp parallel
    {
        const int numThreads  = omp_get_num_threads();
        const int threadID    = omp_get_thread_num();
        const int block       = (numProperties.size()) / numThreads + 1;
        const size_t startIdx = threadID * block;
        const size_t endIdx   = std::min<size_t>(numProperties.size(),
                                                 startIdx + block);

        for (size_t i = startIdx; i < endIdx; ++i) {
            PropertiesChecker::checkProperties(numProperties[i]);
        }
    }

    printResult(numProperties);

    return 0;
}
