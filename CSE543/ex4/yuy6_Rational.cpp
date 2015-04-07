// Copyright (C) 2014 raodm@miamiOH.edu

#include <vector>
#include <algorithm>
#include <iterator>
#include "Rational.h"

// Forward declarations (aka protypes) for unit test methods
void partA_Tests();
void partB_Tests();

// Uncommen the following line for Part B tests.
 #define PARTB_TESTS

int main() {
    // Uncomment following line for Part A. Comment it back for Part B.
    // partA_Tests();
    // Uncomment following line for Part B. Leave it comment for Part A.
    partB_Tests();
    return 0;
}

// Add necessary method definitions for Part B below this line.  The
// addition operator is already implemented to illustrate an
// example. Other operators are also 1/2 line methods. Practice
// implementing them once you have completed Part A.

Rational
Rational::operator+(const Rational& other) const {
    return Rational((numerator   * other.denominator) +
                    (denominator * other.numerator),
                    denominator * other.denominator);
}


//----------------------------------------------------------------
// DO NOT MODIFY CODE BELOW THIS LINE.  However, study it!
//----------------------------------------------------------------

// Definition for the constant that specifies the maximum random value
const int Rational::MaxRandomVal = 10000;

// The default constructor
Rational::Rational() {
    numerator   = rand() % MaxRandomVal;
    denominator = 1 + (rand() % MaxRandomVal);    
}

// A non-default constructor
Rational::Rational(const int numerator, const int denominator) :
    numerator(numerator), denominator(denominator) {
    // Nothing else to be done here.
}

// Stream insertion operator.
std::ostream& operator<<(std::ostream& os, const Rational& r) {
    return (os << r.numerator << "/" << r.denominator);
}

bool
Rational::isPrime(const int number) {
    const int MaxFactor = std::sqrt(number);
    for (int factor = 2; (factor < MaxFactor); factor++) {
        if (number % factor == 0) {
            // Found a factor. So number is not prime.
            return false;
        }
    }
    // prime number.
    return true;
}

int
Rational::getPrimeFactorCount(const int number) {
    int factorCount = 0;
    for (int factor = 1; (factor <= number); factor++) {
        if (((number % factor) == 0) && isPrime(factor)) {
            factorCount++;
        }
    }
    return factorCount;
}

Rational
Rational::toPrimeFactorCount() const {
    return Rational(getPrimeFactorCount(numerator),
                    getPrimeFactorCount(denominator));
}

bool
Rational::operator<(const Rational& rhs) const {
    return static_cast<double>(*this) < static_cast<double>(rhs);
}

bool
Rational::customCompare(const Rational& lhs, const Rational& rhs) {
    return lhs.toPrimeFactorCount() < rhs.toPrimeFactorCount();
}

Rational&
Rational::operator+=(const Rational& other) {
    return (*this = *this + other);
}

Rational
Rational::operator*(const Rational& other) const {
    return Rational(numerator * other.numerator,
                    denominator * other.denominator);
}

Rational
Rational::operator*(const int scale) const {
    return Rational(numerator * scale, denominator * scale);
}

bool
Rational::operator==(const Rational& other) const {
    return static_cast<double>(*this) == static_cast<double>(other);
}

#ifdef PARTB_TESTS

/** This unit test method is to test implementation of addition,
    subtraction, multiplication, and division operators.

*/
void partB_Tests() {
    Rational r1(1, 2), r2(1, 2), r3(4, 5);
    // Test operator+=
    r3 += r1;
    // Print results
    std::cout << std::boolalpha;
    std::cout << "r1 + r2 : " << r1 << " +  " << r2 << " = "
              << (r1 + r2)    << std::endl;
    std::cout << "r1 * r2 : " << r1 << " *  " << r2 << " = "
              << (r1 * r2)    << std::endl;
    std::cout << "r1 * 10 : " << r1 << " *  " << 10 << "  = "
              << (r1 * 10)    << std::endl;
    std::cout << "r3      : " << r3 << std::endl;    
    std::cout << "r1 == r2: " << r1 << " == " << r2 << " = "
              << (r1 == r2)   << std::endl;
    std::cout << "r1 == r3: " << r1 << " == " << r3 << " = "
              << (r1 == r3)   << std::endl;
}

#endif

void partA_Tests() {
    // Initialize random number generator (so all students get same numbers)
    srand(0);
    // Create twenty thousand random rational numbers for testing.    
    std::vector<Rational> numbers(20000);
    // Sort the rational numbers based on factor counts using custom comparator
    std::sort(numbers.begin(), numbers.end(), Rational::customCompare);
    // Print the top 10 values.
    std::copy_n(numbers.begin(), 10,
                std::ostream_iterator<Rational>(std::cout, "\n"));
}
