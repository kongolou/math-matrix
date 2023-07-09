
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <iomanip>
#include <cmath>


class Matrix
{
private:
    int row;
    int column;
    double** pointer;
protected:
    void Initialize();
    void Create();
    void Destroy();
public:
    Matrix();
    explicit Matrix(const int& dimension);
    Matrix(const int& row, const int& column);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other);
    ~Matrix();
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other);
    Matrix operator+(const Matrix& other);
    Matrix operator-(const Matrix& other);
    Matrix operator*(const Matrix& other);
    friend Matrix operator*(const double& number, const Matrix& matrix);
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
    void MakeZero();
    void MakeIdentity();
    void MakeFromSequence(const double* sequence);
    double Determinant();
    double Cofactor(const int& x, const int& y);
    Matrix Transpose();
    Matrix AdjointMatrix();
    Matrix Inverse();
};


#endif // MATRIX_H
