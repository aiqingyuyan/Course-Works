/**
 *  Copyright [2015] <Yan Yu>
 */

#include <iostream>
#include <cmath>
#include "PropertiesChecker.h"

void
PropertiesChecker::checkProperties(numProperties& num) {
    num._isPrime               = isPrime(num._num);
    num._isPalindromic         = isPalindromic(num._num);
    if (!num._isPrime) {
        num._isPrimeFactorable = isPrimeFactorable(num._num);
        num._isFactorial       = isFactorial(num._num);
    }
}

bool
PropertiesChecker::isPrime(const long long& num) {
    if (num <= 1)
        return false;

    long long upperBound = std::sqrt(num);
    for (long long i = 2; i <= upperBound; ++i) {
        if (num % i == 0)
            return false;
    }

    return true;
}

bool
PropertiesChecker::isPrimeFactorable(const long long& num) {
    long long upperBound = std::sqrt(num);
    for (long long i = 2; i <= upperBound; ++i) {
        if (num % i == 0) {
            if (isPrime(i)) {
               if (isPrime(num / i))
                   return true;
           }
        }
    }
    return false;
}

bool
PropertiesChecker::isPalindromic(const long long& num) {
    if (num < 0)
        return false;
    if (num > 0 && num < 10)
        return true;

    long long copy = num, divider = 1;
    while (copy / divider >= 10) {
        divider *= 10;
    }
    unsigned int head, tail;
    while (copy >= 10) {
        head = copy / divider;
        tail = copy % 10;
        if (head != tail)
            return false;
        copy -= head * divider;
        copy /= 10;
    }
    return true;
}

bool
PropertiesChecker::isFactorial(const long long& num) {
    long long copy = num;
    long i = 2;
    while (copy > 1) {
        if (copy % i == 0) {
            copy /= i;
            ++i;
        } else {
            return false;
        }
    }
    return true;
}
