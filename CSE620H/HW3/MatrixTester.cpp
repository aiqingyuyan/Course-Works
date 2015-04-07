// Copyright [2014] raodm@miamiOH.edu

#include <fstream>
#include <string>
#include "Matrix.h"

Matrix createTestMatrix() {
    Matrix a(1, 3);
    a(0, 0) = 1;  // Test operator()(unsigned, unsigned)
    a(0, 1) = 2;
    a(0, 2) = 3;
    return a;
}

void staticTests() {
    const Matrix a = createTestMatrix();
    const Matrix b = ~a;  // Test the transpose operator.
    std::cout << "Matrix a:\n" << a
              << "Matrix b:\n" << b;
    // Multiplication and copy-constructor test
    Matrix c = b * a;
    // Test of stream insertion operator.
    std::cout << "c = b * a:\n" << c.setFormat(3, 1);
    // Test of column insert operator
    c = (c >> 3);
    std::cout << "After inserting 3 columns, Matrix c = \n" << c;
    // Test of column delete operator
    std::cout << "After deleting 2 columns, Matrix c = \n"
              << (c = c << 2);
    // Test of copy constructor and scalar multiplication & division
    Matrix d = (a * 4) / 2.5;
    std::cout << "(a * 4) / 2.5:\n" << d.setFormat(6, 2);
    // Test compound scalar operations on matrices
    d /= 2.5;  // Do compound division.
    std::cout << "d /= 2.5:\n" << d;
    d *= 1;
    std::cout << "d *= 1.0:\n" << d;
}

void testWithData(const std::string& matFile1, const std::string& matFile2) {
    Matrix a, b;
    std::ifstream mat1Data(matFile1), mat2Data(matFile2);
    mat1Data >> a;
    std::cout << "Matrix a (from: " << matFile1 << "):\n" << a.setFormat(8, 4);
    mat2Data >> b;
    std::cout << "Matrix b (from: " << matFile2 << "):\n" << b.setFormat(8, 4);
    Matrix c = a * b;
    std::cout << "Matrix c (a * b):\n" << c.setFormat(8, 4);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        staticTests();
    } else {
        testWithData(argv[1], argv[2]);
    }
    return 0;
}
