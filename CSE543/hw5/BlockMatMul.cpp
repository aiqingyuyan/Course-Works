#ifndef BLOCK_MAT_MUL_CPP
#define BLOCK_MAT_MUL_CPP

#include <string>
#include <fstream>
#include "Matrix.h"

/** A simple test for benchmarking Block matrix multiplication

    Copyright (C) 2015 raodm@miamiOH.edu

    This class is synonymous to MatMul.cpp except it uses the block
    matrix multiplication supported by Matrix.h
*/

#define MATRIX_SIZE 3000
#define REPETITIONS 1

int main() {
    std::cout << "Running " << REPETITIONS << " repetitions of "
              << MATRIX_SIZE << "x" << MATRIX_SIZE
              << " matrix multiplication.\n";
    Matrix::setBlockSize(50);
    auto init = [](int row, int col) { return row + col; };
    // Repeatedly perform the benchmark tests
    const Matrix matrix1(MATRIX_SIZE, MATRIX_SIZE, init);
    const Matrix matrix2(MATRIX_SIZE, MATRIX_SIZE, init);
    for (size_t i = 0; (i < REPETITIONS); i++) {
        const Matrix matrix3 = matrix1 * matrix2;
        std::cout << "Diag sum: " << matrix3.sumDiag() << std::endl;
    }    
    return 0;
}
#endif
