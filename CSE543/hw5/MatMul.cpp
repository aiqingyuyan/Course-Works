#include <iostream>

/** A simple matrix multiplication program.

    This program has been designed to provide a very simple matrix
    multiplication program for benchmarking etc.

    Copyright (C) 2015 raodm@miamiOH.edu
*/

#define MATRIX_SIZE 3000
#define REPETITIONS 1

int matrix1[MATRIX_SIZE][MATRIX_SIZE];
int matrix2[MATRIX_SIZE][MATRIX_SIZE];
int matrix3[MATRIX_SIZE][MATRIX_SIZE];

void initMatrix(int matrix[][MATRIX_SIZE]) {
    for (int row = 0; (row < MATRIX_SIZE); row++) {
        for (int col = 0; (col < MATRIX_SIZE); col++) {
            matrix[row][col] = row + col;
        }
    }
}

void multiply(int matrix1[][MATRIX_SIZE],
              int matrix2[][MATRIX_SIZE],
              int matrix3[][MATRIX_SIZE]) {
    for (int row = 0; (row < MATRIX_SIZE); row++) {
        for (int col = 0; (col < MATRIX_SIZE); col++) {
            int sum = 0;
            for (int i = 0; (i < MATRIX_SIZE); i++)  {
                sum += (matrix1[row][i] * matrix2[i][col]);
            }
            matrix3[row][col] = sum;
        }
    }
}

int sumDiag(int matrix1[][MATRIX_SIZE]) {
    int sum = 0;
    for (int i = 0; (i < MATRIX_SIZE); i++) {
        sum += matrix1[i][i];
    }
    return sum;
}

int main() {
    std::cout << "Running " << REPETITIONS << " repetitions of "
              << MATRIX_SIZE << "x" << MATRIX_SIZE
              << " matrix multiplication.\n";
    initMatrix(matrix1);
    initMatrix(matrix2);
    for (int i = 0; (i < REPETITIONS); i++) {
        multiply(matrix1, matrix2, matrix3);
        std::cout << "Diag sum: " << sumDiag(matrix3) << std::endl;
    }
}

// End of source code.
