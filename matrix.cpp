
#include "matrix.h"

void Matrix::Initialize()
{
    row = 0;
    column = 0;
    pointer = nullptr;
}

void Matrix::Create()
{
    pointer = new double* [row];
    for (int i = 0; i < row; ++i)
    {
        pointer[i] = new double[column];
    }
    MakeZero();
}

void Matrix::Destroy()
{
    for (int i = 0; i < row; ++i)
    {
        delete[] pointer[i];
    }
    delete[] pointer;
    Initialize();
}

Matrix::Matrix()
{
    Initialize();
}

Matrix::Matrix(const int& dimension)
    : row(dimension), column(dimension)
{
    Create();
}

Matrix::Matrix(const int& row, const int& column)
    : row(row), column(column)
{
    Create();
}

Matrix::Matrix(const Matrix& other)
    : row(other.row), column(other.column)
{
    Create();
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < column; ++j)
        {
            pointer[i][j] = other.pointer[i][j];
        }
    }
}

Matrix& Matrix::operator=(const Matrix& other)
{
    if (this != &other)
    {
        Destroy();
        Matrix(other.row, other.column);
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < column; ++j)
            {
                pointer[i][j] = other.pointer[i][j];
            }
        }
    }
    return *this;
}

Matrix::Matrix(Matrix&& other)
    : row(other.row), column(other.column), pointer(other.pointer)
{
    other.Initialize();
}

Matrix& Matrix::operator=(Matrix&& other)
{
    if (this != &other)
    {
        Destroy();
        row = other.row;
        column = other.column;
        pointer = other.pointer;
        other.Initialize();
    }
    return *this;
}

Matrix::~Matrix()
{
    Destroy();
}

Matrix Matrix::operator+(const Matrix& other)
{
    Matrix result{row, column};
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < column; ++j)
        {
            result.pointer[i][j] = pointer[i][j] + other.pointer[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other)
{
    Matrix result{row, column};
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < column; ++j)
        {
            result.pointer[i][j] = pointer[i][j] - other.pointer[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other)
{
    Matrix result{row, other.column};
    for (int i = 0; i < result.row; ++i)
    {
        for (int j = 0; j < result.column; ++j)
        {
            for (int k = 0; k < column; ++k)
            {
                result.pointer[i][j] += pointer[i][k] * other.pointer[k][j];
            }
        }
    }
    return result;
}

Matrix operator*(const double& number, const Matrix& matrix)
{
    Matrix result{matrix.row, matrix.column};
    for (int i = 0; i < matrix.row; ++i)
    {
        for (int j = 0; j < matrix.column; ++j)
        {
            result.pointer[i][j] = number * matrix.pointer[i][j];
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
{
    for (int i = 0; i < matrix.row; ++i)
    {
        for (int j = 0; j < matrix.column; ++j)
        {
            os << setiosflags(std::ios::fixed) << matrix.pointer[i][j] << "\t";
        }
        os << "\n";
    }
    os << std::endl;
    return os;
}

void Matrix::MakeZero()
{
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < column; ++j)
        {
            pointer[i][j] = 0.0;
        }
    }
}

void Matrix::MakeFromSequence(const double* sequence)
{
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < column; ++j)
        {
            pointer[i][j] = sequence[i * column + j];
        }
    }
}

Matrix Matrix::Transpose()
{
    Matrix result{row, column};
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < column; ++j)
        {
            result.pointer[i][j] = pointer[j][i];
        }
    }
    return result;
}

void Matrix::MakeIdentity()
{
    int dimension{row};
    for (int i = 0; i < dimension; ++i)
    {
        pointer[i][i] = 1.0;
    }
}

double Matrix::Determinant()
{
    double result{1.0};
    int dimension{row};
    Matrix help{dimension};
    help.MakeIdentity();
    for (int i = 0; i < help.row; ++i)
    {
        for (int j = 0; j < help.column; ++j)
        {
            double temp = 0.0;
            for (int k = 0; k < std::min(i, j); ++k)
            {
                temp += help.pointer[i][k] * help.pointer[k][j];
            }
            // Specially
            if (fabs(help.pointer[j][j]) < 1e-6)
            {
                return 0.0;
            }
            help.pointer[i][j] = (pointer[i][j] - temp) / help.pointer[j][j];
            // Confused here? Well, This is the LU decomposition.
            // See more... https://zhuanlan.zhihu.com/p/642225813
            if (i == j)
            {
                result *= help.pointer[i][j];
            }
        }
    }
    return result;
}

double Matrix::Cofactor(const int& x, const int& y)
{
    int dimension{row - 1};
    // Specially
    if (dimension == 0)
    {
        return 1.0;
    }
    Matrix help{dimension};
    for (int i = 0; i < help.row; ++i)
    {
        for (int j = 0; j < help.column; ++j)
        {
            help.pointer[i][j] = pointer[(i < x) ? i : (i + 1)][(j < y) ? j : (j + 1)];
        }
    }
    return help.Determinant();
}

Matrix Matrix::AdjointMatrix()
{
    int dimension{row};
    Matrix result{dimension};
    for (int i = 0; i < result.row; ++i)
    {
        for (int j = 0; j < result.column; ++j)
        {
            result.pointer[j][i] = (((i + j) % 2 == 0) ? 1 : -1) * Cofactor(i, j);
        }
    }
    return result;
}

Matrix Matrix::Inverse()
{
    double det = Determinant();
    if (fabs(det) < 1e-6)
    {
        std::cerr << "Failed to inverse." << std::endl;
        return *this;
    }
    return 1 / det * AdjointMatrix();
}
