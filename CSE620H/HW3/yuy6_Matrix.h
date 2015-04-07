/*
 *  Copyright [2014] <Yan Yu>
 */
#ifndef _YAN_YU_MATRIX_H_
#define _YAN_YU_MATRIX_H_

#include <vector>
#include <iostream>
#include <iomanip>
#include <functional>

class Matrix {
private:
    std::vector<std::vector<double>> matrix;
    int rows, columns;
    int width, precision;

protected:
    Matrix binaryOp(Matrix& m,
                   std::function<double(double, double)> op);

public:
    // constructors
    Matrix();
    Matrix(int rows, int columns);
    Matrix(const Matrix& m);

    // functor used to accessed individual element in matrix
    double& operator()(int i, int j);
    // matrix addition
    Matrix  operator+(Matrix& m);
    // matrix subtraction
    Matrix  operator-(Matrix& m);
    // matrix multiplication
    Matrix  operator*(const Matrix& m) const;
    // scalar multiplication
    Matrix  operator*(const double& scalar) const;
    Matrix& operator*=(const double& scalar);

    Matrix  operator/(const double& scalar);
    Matrix& operator/=(const double& scalar);
    // transpose
    Matrix  operator~() const;
    // insert column vectors
    Matrix& operator>>(int num);
    // delete column vectors
    Matrix& operator<<(int num);

    // overload stream insertion and extraction operators
    friend std::ostream& operator<<(std::ostream& out, const Matrix& m);
    friend std::istream& operator>>(std::istream& in, Matrix& m);
    Matrix& setFormat(int w, int p);
};

#endif   // _YAN_YU_MATRIX_H_
