#ifndef RATIONAL_H
#define RATIONAL_H

// Copyright (C) 2014 raodm@miamiOH.edu
    
#include <iostream>

/** A class to reprsent a rational number with a numerator and a
    non-zero denominator.

    This class provides a convenient representation for rational
    numbers to make operations with rational numbers convenient.
*/
class Rational {
    friend std::ostream& operator<<(std::ostream& os, const Rational& r);

private:
    // The numerator for the rational number.
    long numerator;
    // The denominator for the rational number.
    long denominator;
    // The maximum numbers to be randomly generated
    static const int MaxRandomVal;

public:
    /**
       The default constructor to create a random rational number.
       The constructor initializes numerator to a random value (0 to
       MaxRandomVal) and denominator to a random value (1 to
       MaxRandomValue).
    */
    Rational();

    /** Non-default constructor to explicitly initialize numerator and
        denominator.

        \param[in] numerator The numerator value to be set for the
        Rational numebr.

        \param[in] denominator The denominator value to be set for the
        Rational numebr.
    */
    Rational(const int numerator, const int denominator);

    /**
       A convenience method to type-cast a Rational object to a double
       value.

       \return Returns the value of the rational number as a
       double. For example given the rational number 1/2 this method
       will return 0.5.
    */
    operator double() const {
        return numerator / static_cast<double>(denominator);
    }

    /** Method to convert the rational number to ratios of number of
        prime factors for the numerator and denominator.

        For example, given the rational number 10/20, this method
        returns 3/3, becaue 10 has 3 prime factors {1, 2, 5} and 20
        has 5 prime factors {1, 2, 5}.

        \note This method uses the getPrimeFactorCount() method to
        determine count of prime factors.
	
        \return A rational number representing ratios of number of
        prime factors for the given number.
    */
    Rational toPrimeFactorCount() const;

    /** A simple comparison operator that returns true if this
        Rational number is less than rhs.

        \param[in] rhs The right hand side (or other) rational number
        to compare against.

        \return This method returns true if this (Rational object) is
        less than rhs.
    */
    bool operator<(const Rational& rhs) const;

    /** A special comparator to compare prime factor count
        representations of two given Rational numbers.

        \param[in] lhs The logical left-hand-side Rational object.

        \param[in] rhs The logical right-hand-side Rational object.
	
        \return This method returns true if lhs.toPrimeFactorCount() <
        rhs.toPrimeFactorCount().
    */
    static bool customCompare(const Rational& lhs, const Rational& rhs);

    /** Addition operator to add a given Rational number to \c this to
        create a new rational number.

        If \c this is 2/3 and other is 4/7 then the method returns a
        Rational object with value (2*7)+(4*3)/(3*7) = 26/21.
	
        \param[in] other The other rational number to be used for
        addition.

        \return A Rational resulting from adding other to \c this.
    */
    Rational operator+(const Rational& other) const;

    /** Self-addition operator to add a given Rational number to this.

        If \c this is 2/3 and other is 4/7 then the method changes the
        value of \c this to (2*7)+(4*3)/(3*7) = 26/21.

        \return Operators of this form (+=, *=, /= etc.) always
        returns \c this object as per convention.
    */
    Rational& operator+=(const Rational& other);
    

    /** Multiplication operator to multiple a given Rational number to
        \c this to create a new rational number.

        If \c this is 2/3 and other is 4/7 the method returns a
        Rational object with value (2*4)/(3*7) = 8/21.

        \return A Rational resulting from multiplying \c this by
        other.
    */
    Rational operator*(const Rational& other) const;

    /** Multiplication operator to scale this Rational object.

        If \c this is 2/3 and scale is 10 then the method returns a
        Rational object with value 20/30.

        \param[in] scale The scale to be used. If the scale value is
        zero, then it is simply assumed to be 1.
	
        \return A Rational resulting from multiplying numerator and
        denominator by scale.
    */
    Rational operator*(const int scale) const;

    /** Method to check if \c this Rational object is equal to
        another.
        
        \param[in] other The other Rational object for comparison

        \return This method returns true only if the numerator and
        denominator of \c this and other are equal.
     */
    bool operator==(const Rational& other) const;
    
protected:
    /** Method to determine if a given number is prime.

        \param[in] number The number to be checked if it is prime.

        \return This method returns true if the number is
        prime. Otherwise this method returns false.
    */
    static bool isPrime(const int number);

    /** Method to determine the number of prime factors for a given
        number.

        \note This method uses the isPrime() method in this class.
	
        \param[in] number The number whose prime factors are to be
        counted.

        \return The count of prime factors for the given number.
    */
    static int getPrimeFactorCount(const int number);
};

#endif
