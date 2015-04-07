/*
 *  Copyright [2014] <Yan Yu>
 */

#include "./Matrix.h"
#include <vector>
#include <functional>
#include <algorithm>

Matrix::Matrix() : rows(1), columns(1), width(8), precision(5) {
    matrix = std::vector<std::vector<double>>(1, std::vector<double>(1));
}

Matrix::Matrix(int rows, int columns) : width(8), precision(5) {
    matrix = std::vector<std::vector<double>>
                         (rows, std::vector<double>(columns));
    this->rows = rows;
    this->columns = columns;
}

Matrix::Matrix(const Matrix& m) : rows(m.rows), columns(m.columns),
                                  width(8), precision(5) {
    matrix = std::vector<std::vector<double>>(m.matrix);
}

double& Matrix::operator()(int i, int j) {
    // if ((i <= this->rows - 1) &&
    //      j <= this->columns - 1) {
    return matrix[i][j];
    // }
    // return
}

Matrix Matrix::binaryOp(Matrix& m,
                std::function<double(double, double)> op) {
    Matrix tmp(rows, columns);

    for (int i = 0; i < m.rows; i++) {
        std::transform(this->matrix[i].begin(), this->matrix[i].end(),
                        m.matrix[i].begin(), tmp.matrix[i].begin(), op);
    }

    return tmp;
}

Matrix Matrix::operator+(Matrix& m) {
    if (m.rows == rows && m.columns == columns) {
        // Matrix tmp(rows, columns);

        // for (int i = 0; i < rows; i++) {
        //    for (int j = 0; j < columns; j++)
        //        tmp(i, j) = m(i, j) + (*this)(i, j);
        // }

        // return tmp;
        return binaryOp(m, std::plus<double>());
    }

    return Matrix(0, 0);
}

Matrix Matrix::operator-(Matrix& m) {
    if (m.rows == rows && m.columns == columns) {
        // Matrix tmp(rows, columns);

        // for (int i = 0; i < rows; i++) {
        //    for (int j = 0; j < columns; j++)
        //        tmp(i, j) = m(i, j) - (*this)(i, j);
        // }

        // return tmp;
        return binaryOp(m, std::minus<double>());
    }

    return Matrix(0, 0);
}

Matrix Matrix::operator*(const Matrix& m) const {
    if (columns == m.rows) {
        Matrix tmp(rows, m.columns);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < m.columns; j++) {
                double sum = 0;
                for (int p = 0; p < columns; p++) {
                    sum += matrix[i][p] * m.matrix[p][j];
                }
                tmp(i, j) = sum;
            }
        }

        return tmp;
    }

    return Matrix(0, 0);
}

Matrix Matrix::operator*(const double& scalar) const {
    Matrix tmp(rows, columns);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++)
            tmp(i, j) = matrix[i][j] * scalar;
    }

    return tmp;
}

Matrix& Matrix::operator*=(const double& scalar) {
    // for (auto& r : matrix) {
    //    for (auto& e : r)
    //        e *= scalar;
    // }

    return *this = *this * scalar;
}

Matrix Matrix::operator/(const double& scalar) {
    if (scalar != 0) {
        Matrix tmp = (*this) * (1 / scalar);
        return tmp;
    }

    return Matrix(0, 0);
}

Matrix& Matrix::operator/=(const double& scalar) {
    // if (scalar != 0) {
        *this *= (1 / scalar);
        return *this;
    // }
}

Matrix Matrix::operator~() const {
    Matrix tmp(columns, rows);

    for (int i = 0 ; i < tmp.rows; i++) {
        for (int j = 0; j < tmp.columns; j++) {
            tmp(i, j) = matrix[j][i];
        }
    }

    return tmp;
}

Matrix& Matrix::operator>>(int num) {
    columns += num;

    for (auto& row : matrix) {
        for (int i = 0; i < num; i++)
            row.insert(row.begin(), 0);
    }

    return *this;
}

Matrix& Matrix::operator<<(int num) {
    // std::vector<std::vector<double>>::const_iterator it = matrix.begin();
    columns -= num;

    for (auto& row : matrix) {
        for (int i = 0; i < num; i++)
            row.erase(row.begin());
    }

    return *this;
}

Matrix& Matrix::setFormat(int w, int p) {
    width = w;
    precision = p;

    return *this;
}

std::ostream& operator<<(std::ostream& out, const Matrix& m) {
    for (auto& row : m.matrix) {
        for (auto& e : row) {
            out << std::setw(m.width)
                << std::setprecision(m.precision)
                << e << " ";
        }
        out << std::endl;
    }
    return out;
}

std::istream& operator>>(std::istream& in, Matrix& m) {
    int rows, columns;
    in >> rows >> columns;
    m = Matrix(rows, columns);

    for (auto& row : m.matrix) {
        for (auto& e : row)
            in >> e;
    }

    return in;
}

