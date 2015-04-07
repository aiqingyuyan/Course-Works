/**
 *  Copyright [2015] <Yan Yu>
 */

#ifndef _PROPERTIES_CHECKER_H_
#define _PROPERTIES_CHECKER_H_

#include "numProperties.h"

class PropertiesChecker {
public:
    PropertiesChecker() = delete;
    PropertiesChecker(PropertiesChecker&) = delete;
    PropertiesChecker(PropertiesChecker&&) = delete;
    ~PropertiesChecker() = delete;

    static void checkProperties(numProperties& num);

protected:
    static bool isPrime(const long long& num);
    static bool isPrimeFactorable(const long long& num);
    static bool isPalindromic(const long long& num);
    static bool isFactorial(const long long& num);
};

#endif
