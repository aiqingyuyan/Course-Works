// Copyright <Yan Yu>, Miami University 2014
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include "Matrix.h"

int Matrix::blockSize = 1;

Matrix::Matrix() : data(1, std::vector<int>(1, 0)) { }

Matrix::Matrix(int rows, int cols) :
                  data(rows, std::vector<int>(cols, 0)) { }

Matrix::Matrix(int rows, int cols,
               std::function<Val(int, int)> init) :
                  data(rows, std::vector<int>(cols)) {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            data[row][col] = init(row, col);
        }
    }
}

Matrix::Matrix(const Matrix& src) : data(src.data) { }

Matrix::Matrix(Matrix&& src) : data(std::move(src.data)) { }

Matrix
Matrix::operator+(const Matrix& src) const throw(std::exception) {
    if (data.size() != src.data.size() ||
        data[0].size() != src.data[0].size()) {
            std::cerr << *this << "\n\n" << src;
            throw std::runtime_error("+ dimensions error");
    }

    Matrix result(data.size(), data[0].size());
    for (size_t row = 0; row < data.size(); ++row) {
        result.data[row].data()[0:data[0].size()]
               = data[row].data()[0:data[0].size()] +
                 src.data[row].data()[0:data[0].size()];
    }

    return result;
}

Matrix
Matrix::operator*(const Matrix& src) const throw(std::exception) {
    if (data[0].size() != src.data.size()) {
        throw std::runtime_error("* dimensions error");
    }

    size_t m1_rows = data.size(), m1_cols = data[0].size();
    size_t m2_cols = src.data[0].size();

    Matrix result(m1_rows, m2_cols);

    for (size_t i = 0; i < m1_rows; i += blockSize) {
        for (size_t j = 0; j < m2_cols; j += blockSize) {
            for (size_t k = 0; k < m1_cols; k += blockSize) {
                multiply(data, src.data, result.data,
                         i, k, k, j);
            }
        }
    }

    return result;
}

void
Matrix::multiply(const TwoDVec& matrix1, const TwoDVec& matrix2,
                 TwoDVec& matrix3, size_t startRow1, size_t startCol1,
                 size_t startRow2, size_t startCol2) const {
    size_t m1_row_bound = std::min(startRow1 + blockSize,
                                   matrix1.size());
    size_t m1_col_bound = std::min(startCol1 + blockSize,
                                   matrix1[0].size());
    size_t m2_col_bound = std::min(startCol2 + blockSize,
                                   matrix2[0].size());

    for (size_t i = startRow1; i < m1_row_bound; ++i) {
        for (size_t j = startCol2; j < m2_col_bound; ++j) {
            Val sum = 0;
            for (size_t k = startCol1; k < m1_col_bound; ++k) {
                sum += matrix1[i][k] * matrix2[k][j];
            }
            matrix3[i][j] += sum;
        }
    }
}

Matrix&
Matrix::operator=(const Matrix& src) {
    data = src.data;
    return *this;
}

Val
Matrix::sumDiag() const {
    size_t dim = std::min(data.size(), data[0].size());

    Val sum = 0;
    for (size_t i = 0; i < dim; ++i) {
        for (size_t j = 0; j < dim; ++j) {
            sum += data[i][j];
        }
    }

    return sum;
}

std::ostream&
operator<<(std::ostream& os, const Matrix& matrix) {
    os << matrix.data.size() << " " << matrix.data[0].size() << "\n";
    size_t rows = matrix.data.size(), cols = matrix.data[0].size();

    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            os << matrix.data[row][col] << " ";
        }
        os << "\n";
    }

    return os;
}

std::istream&
operator>>(std::istream& is, Matrix& matrix) {
    size_t rows, cols;
    is >> rows >> cols;
    matrix.data.resize(rows);

    for (size_t row = 0; row < rows; ++row) {
        matrix.data[row].resize(cols);
        for (size_t col = 0; col < cols; ++col) {
            is >> matrix.data[row][col];
        }
    }

    return is;
}
