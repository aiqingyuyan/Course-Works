/**
 *  Copyright [2015] <Yan Yu>
 */

#ifndef _NUM_PROPERTIES_H_
#define _NUM_PROPERTIES_H_

typedef struct _numProperties {
    long long _num;
    bool _isPrime, _isPrimeFactorable, _isFactorial, _isPalindromic;

    _numProperties() : _num(0) {
        _isPrime = _isPrimeFactorable =
        _isFactorial = _isPalindromic = false;
    }

    _numProperties(long long num) :  _num(num) {
        _isPrime = _isPrimeFactorable =
        _isFactorial = _isPalindromic = false;
    }
} numProperties;

#endif
