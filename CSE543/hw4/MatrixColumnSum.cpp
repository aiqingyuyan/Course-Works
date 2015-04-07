#include <iostream>
#include <vector>
#include <sys/time.h>

// Program to benchmark the effectiveness of cache access patterns

// The matrix size that we are working with. This value can be changed
// depending on the machine or duration of the tests.
const int MatrixSize = 2500;

// The global shared matrix using which the tests are going to be
// performed.
std::vector< std::vector<int> >
matrix( MatrixSize, std::vector<int>(MatrixSize) );

/**
   Fills in the values into the matrix for testing purposes. This
   method is not really being benchmarked but does consume some time
   to run.
*/
void generateMatrix() {
   for(int col = 0; (col < MatrixSize); col++) {
        for(int row = 0; (row < MatrixSize); row++) {
            matrix[row][col] = row + col;
        }
   } 
}


// This approach for computing sum of each column of the given
// matrix.  The code is structured as:
//
//    For each column in the matrix
//        * Compute sum of all rows in the column of the matrix
//
void columnSum1(std::vector<int> &colSum) {
    colSum.resize(MatrixSize);
    for(int col = 0; (col < MatrixSize); col++) {
        colSum[col] = 0;
        for(int row = 0; (row < MatrixSize); row++) {
            colSum[col] += matrix[row][col];
        }
    }
}

// This approach for computing sum of each column of the given
// matrix.  The code is structured as:
//
//    For each row in the matrix
//        * Update current sum for all columns in the matrix
//
void columnSum2(std::vector<int> &colSum) {
    colSum.clear();
    colSum.resize(MatrixSize);
    for(int row = 0; (row < MatrixSize); row++) {
        for(int col = 0; (col < MatrixSize); col++) {
            colSum[col] += matrix[row][col];
        }
    }
}

int main() {
    generateMatrix();    
    std::vector<int> colSum(MatrixSize);
    for(int i = 0; (i < 100); i++) {
        columnSum2(colSum);
    }
    std::cout << "colSum[10] = " << colSum[10] << std::endl;
    return 0;
}
