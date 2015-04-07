#ifndef MATRIX_TESTER_CPP
#define MATRIX_TESTER_CPP

#include <string>
#include <fstream>
#include "Matrix.h"

/** A simple class to invoke various methods in the Matrix class.

    Copyright (C) 2015 raodm@miamiOH.edu
    
    Calling this file a "tester" is a bit of a misnomer.  It really
    does not test (to avoid one of the common mistakes in Unit tests)
    but rather exercises the various API methods in the matrix class.
    It generates outputs from the various operations which can then be
    compared with a reference output for comparisons.  This class is
    meant to be used in the following manner:

    $ icpc -g -Wall -std=c++11 -O2 Matrix.cpp MatrixTester.cpp -o MatrixTester
    $ ./MatrixTester /shared/raodm/csex43/data/hw5_mat_data.txt  > my_output.txt
    $ diff my_output.txt /shared/raodm/csex43/data/hw5_mat_ref_output.txt
*/

/** A simple test method to verify matrix operations.  This method
    invokes/utilizes (not really test) the following features:

    1. Matrix constructors.
    2. Matrix addition
    3. Matrix multiplication
    4. Stream insertion operator
    5. Stream extraction operator.
    6. Assignment operator.
*/
void runMethods(const std::string& path) {
    const Matrix m1(3, 7, [](int row, int col) { return row + col; });
    const Matrix m2(7, 3, [](int row, int col) { return row; });
    const Matrix m3(3, 7, [](int row, int col) { return row; });
    // Verify the addition operation.
    Matrix result = m1 + m3;
    std::cout << result << std::endl;    // Print results
    // Use operations with different block sizes
    for (int blkSize = 1; (blkSize < 10); blkSize++) {
        Matrix::setBlockSize(blkSize);  // Setup block size.
        Matrix result(10, 10);          // Test matrix
        result = m1 * m2;               // Perform matrix multiplication
        std::cout << result << std::endl;
    }
    // Use I/O methods
    std::ifstream dataFile(path);
    if (!dataFile.good()) {
        throw std::runtime_error("Invalid matrix data file");
    }
    // Load data for matrices from files.
    Matrix m4, m5(7, 5), m6;
    dataFile >> m4 >> m5 >> m6;  // Read matrices from file
    std::cout << (m4 + m6) << std::endl
              << (m4 * m5) << std::endl;
}

int
main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Specify input matrix data file\n" << std::endl;
        return 1;
    }
    runMethods(argv[1]);
    return 0;
}

#endif
