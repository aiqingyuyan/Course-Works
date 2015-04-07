#ifndef MATRIX_H
#define MATRIX_H

// Copyright (C) 2015 raodm@miamiOH.edu

#include <iostream>
#include <functional>
#include <vector>

/** Shortcut for the value of each element in the matrix */
using Val = int;

/** Short cut to a 2-d vector double values to streamline the code */
using TwoDVec = std::vector<std::vector<Val>>;

/** A matrix class to perform basic matrix operations.

    The class essentially encapsulates a 2-d matrix of double values
    and performs the following matrix operations:

    <ul>
    
    <li>Create a matrix of given dimensions.</li>

    <li>Matrix addition using Cilk array notation.<li>

    <li>Matrix multiplication using Block matrix multiplication.</li>

    <li> Stream insertion and extraction operators to conveniently
    load and print values.</li>
    
    </ul>
*/
class Matrix {
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
    friend std::istream& operator>>(std::istream& is, Matrix& matrix);

public:
    /** Default constructor.

        The constructor creates a matrix of size 1x1 and initializes
        each entry to zero.
    */
    Matrix();
    
    /** Constructor to create matrix of a given size.
        
        \param[in] rows The number of rows in the matrix.
        
        \param[in] cols The number of columns in the matrix.
        
        The constructor initializes each entry to zero.
    */
    Matrix(int rows, int cols);

    /** Constructor to initialize values in a given matrix.

        This constructor accepts a binary function that can be used to
        initialize each entry in the matrix.  The binary function is
        invoked for each entry in the matrix with the row and column
        as the parameter.
        
        \param[in] rows The number of rows in the matrix.
        
        \param[in] cols The number of columns in the matrix.

        \param[in] init Method to initialize each value in the matrix.
        The constructor initializes each entry by calling the initi
        method with the row and col of the entry as the parameters.
    */
    Matrix(int rows, int cols, std::function<Val(int, int)> init);
    
    /** Copy constructor.

        The copy constructor for the matrix class.
        
        \param[in] src The source matrix from where the data is to be
        copied.
    */
    Matrix(const Matrix& src);

    /** The move constructor.

        The move constructor for the matrix class.
        
        \param[in] src The source matrix from where the data is to be
        moved.
    */
    Matrix(Matrix&& src);

    /** Addition operator for the matrix class.

        This operator (or method) performs standard matrix addition,
        except it uses Cilk array notation for perform vector
        operations.
        
        \param[in] src The second matrix to be used for addition.
        This method assumes that the

        \exception std::exception This method throws a
        std::runtime_error if the dimensions of the matrix are not
        compatible.
    */
    Matrix operator+(const Matrix& src) const throw(std::exception);

    /** Matrix multiplication operator for the matrix class.
        
        This operator (or method) performs standard matrix
        multiplication, except it uses block matrix multiplication for
        optimizing matrix multiplication.
        
        \param[in] src The second matrix to be used for addition.
        This method assumes that the
        
        \exception std::exception This method throws a
        std::runtime_error if the dimensions of the matrix are not
        compatible.
    */
    Matrix operator*(const Matrix& src) const throw(std::exception);

    /** The assignment operator.

        A standard assignment operator.

        \param[in] src The source matrix from where the matrix is to
        be copied.
    */
    Matrix& operator=(const Matrix& src);

    /** Returns the diagonal sum of the given matrix.

        This method returns the diagonal sum of the matrix.  If the
        matrix is not a square matrix, then the smaller of the two
        dimensions is used to determine the diagonal.

        \return The sum of the values that lie on the diagonal of this
        matrix.
    */
    Val sumDiag() const;
    
    /** A static method to set the block size to be used for matrix
        multiplication.

        This method must be used to set the block size to be used for
        matrix multiplication.
    */
    static void setBlockSize(int blkSize) { blockSize = blkSize; }
    
protected:
    /**
       This is the core multiplication helper method that is used to
       facilitate block matrix multiplication operations.  This
       function essentially performs block operations of fixed size.
   
       matrix3[startRow:startCol2] = matrix1[startRow1:startCol1] *
                                     matrix2[startRow2:startCol2]
       
       \param[in] matrix1 First matrix that be used in multiplication.
   
       \param[in] matrix2 A second matrix that must be used in
       multiplication.
   
       \param[out] matrix3 The destination matrix that will contain
       the resulting matrix at the end of the matrix multiplication
       operation.
    */
    void multiply(const TwoDVec& matrix1, const TwoDVec& matrix2,
                  TwoDVec& matrix3, size_t startRow1, size_t startCol1,
                  size_t startRow2, size_t startCol2) const;

private:
    /** The block size to be used for matrix multiplication.

        The maximum block size to be used for matrix multiplication.
        In the .cpp file include the following line outside all
        methods to define the variable (for this declaration):

		int Matrix::blockSize = 1;
    */
    static int blockSize;

    /** The 2-d vector of values that is encapsulated by this class.

        The vector is resized as part of the various operations. This
        vector can be initialized (to all zeros) using the following
        code fragment: data(rows, std::vector<Val>(cols, 0))
    */
    TwoDVec data;
};

#endif
