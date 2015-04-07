// Copyright - Yan YU, Miami University 2015

#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
// #define USING_LAMBDA

const long size = 150000000;

struct {
    bool __attribute__((noinline))  operator()(const int& a, const int& b) {
       return a < b;
    }
} customLess;

int main(int argc, char** argv) {
    std::srand(std::time(0));
    std::vector<int> data(size);
    std::generate(data.begin(), data.end(), std::rand);

#ifdef USING_LAMBDA
    std::sort(data.begin(), data.end(),
             [](const int& a, const int& b){
                 return a < b;
             });
#else
    std::sort(data.begin(), data.end(), customLess);
#endif

    return 0;
}
