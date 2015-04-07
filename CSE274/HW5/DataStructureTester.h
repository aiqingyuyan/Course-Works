#include <iostream>
#include <ctime>
#include <cstdlib>
#include "array.h"

namespace ods {

template<class H>
class DataStructureTester {
protected:
    H list;
    array<int> randomAddedNums;
public:
    DataStructureTester() : list(), randomAddedNums(1) {}
    double DoSequentialAdd(int start, int end, int step);
    double DoRandomAdd(int n);
    double DoSequentialRemove(int start, int end, int step);
    double DoRandomRemove(int n);
    double DoSequentialFind(int start, int end, int step);
    double DoRandomFind(int n);
};

template<class H>
double DataStructureTester<H>::DoSequentialAdd(int start, int end, int step) {
    clock_t t;
    t = clock();
    for (int i = start; i <= end; i+= step) {
        list.add(i);
    }
    t = clock() - t;
    return ((double)t / CLOCKS_PER_SEC);
}

template<class H>
double DataStructureTester<H>::DoRandomAdd(int n) {
    clock_t t;
    srand(time(NULL));
    t = clock();
    int i = 0, num;
    array<int> tmp(n);
    while(i < n) {
        num = rand();
        list.add(num);
        tmp[i] = num;
        i++;
    }
    t = clock() - t;
    randomAddedNums = tmp;
    return ((double)t / CLOCKS_PER_SEC);
}

template<class H>
double DataStructureTester<H>::DoSequentialRemove(int start, int end, int step) {
    clock_t t;
    t = clock();
    for (int i = start; i <= end; i+= step) {
        list.remove(i);
    }
    t = clock() - t;
    return ((double)t / CLOCKS_PER_SEC);
}

template<class H>
double DataStructureTester<H>::DoRandomRemove(int n) {
    clock_t t;
    t = clock();
    int i = 0;
    while(i < n) {
        list.remove(randomAddedNums[i]);
        i++;
    }
    t = clock() - t;
    return ((double)t / CLOCKS_PER_SEC);
}

template<class H>
double DataStructureTester<H>::DoSequentialFind(int start, int end, int step) {
    clock_t t;
    t = clock();
    for (int i = start; i <= end; i+= step) {
        list.find(i);
    }
    t = clock() - t;
    return ((double)t / CLOCKS_PER_SEC);
}

template<class H>
double DataStructureTester<H>::DoRandomFind(int n) {
    clock_t t;
    t = clock();
    int i = 0;
    while(i < n) {
        list.find(randomAddedNums[i]);
        i++;
    }
    t = clock() - t;
    return ((double)t / CLOCKS_PER_SEC);
}

}
