
#include "matrix.h"

void Matrix::Initialize() {
  row = 0;
  column = 0;
  pointer = nullptr;
}

void Matrix::Create() {
  pointer = new double *[row];
  for (int i = 0; i < row; ++i) {
    pointer[i] = new double[column];
  }
  MakeItZero();
}

void Matrix::Destroy() {
  for (int i = 0; i < row; ++i) {
    delete[] pointer[i];
  }
  delete[] pointer;
  Initialize();
}

Matrix::Matrix() { Initialize(); }

Matrix::Matrix(const int &dimension) : row(dimension), column(dimension) {
  Create();
}

Matrix::Matrix(const int &row, const int &column) : row(row), column(column) {
  Create();
}

Matrix::Matrix(const Matrix &other) : row(other.row), column(other.column) {
  Create();
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      pointer[i][j] = other.pointer[i][j];
    }
  }
}

Matrix &Matrix::operator=(const Matrix &other) {
  if (this != &other) {
    Destroy();
    Matrix(other.row, other.column);
    for (int i = 0; i < row; ++i) {
      for (int j = 0; j < column; ++j) {
        pointer[i][j] = other.pointer[i][j];
      }
    }
  }
  return *this;
}

Matrix::Matrix(Matrix &&other)
    : row(other.row), column(other.column), pointer(other.pointer) {
  other.Initialize();
}

Matrix &Matrix::operator=(Matrix &&other) {
  if (this != &other) {
    Destroy();
    row = other.row;
    column = other.column;
    pointer = other.pointer;
    other.Initialize();
  }
  return *this;
}

Matrix::~Matrix() { Destroy(); }

Matrix Matrix::operator+(const Matrix &other) {
  if (!(row == other.row && column == other.column)) {
    throw "Unmatched size. Failed to +.";
  }
  Matrix result{row, column};
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      result.pointer[i][j] = pointer[i][j] + other.pointer[i][j];
    }
  }
  return result;
}

Matrix Matrix::operator-(const Matrix &other) {
  if (!(row == other.row && column == other.column)) {
    throw "Unmatched size. Failed to -.";
  }
  Matrix result{row, column};
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      result.pointer[i][j] = pointer[i][j] - other.pointer[i][j];
    }
  }
  return result;
}

Matrix Matrix::operator*(const Matrix &other) {
  if (column != other.row) {
    throw "Unmatched size. Failed to *.";
  }
  Matrix result{row, other.column};
  for (int i = 0; i < result.row; ++i) {
    for (int j = 0; j < result.column; ++j) {
      for (int k = 0; k < column; ++k) {
        result.pointer[i][j] += pointer[i][k] * other.pointer[k][j];
      }
    }
  }
  return result;
}

Matrix operator*(const double &number, const Matrix &matrix) {
  Matrix result{matrix.row, matrix.column};
  for (int i = 0; i < matrix.row; ++i) {
    for (int j = 0; j < matrix.column; ++j) {
      result.pointer[i][j] = number * matrix.pointer[i][j];
    }
  }
  return result;
}

std::ostream &operator<<(std::ostream &os, const Matrix &matrix) {
  for (int i = 0; i < matrix.row; ++i) {
    for (int j = 0; j < matrix.column; ++j) {
      os << setiosflags(std::ios::fixed) << matrix.pointer[i][j] << "\t";
    }
    os << "\n";
  }
  os << std::endl;
  return os;
}

void Matrix::MakeItZero() {
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      pointer[i][j] = 0.0;
    }
  }
}

void Matrix::MakeItIdentity() {
  if (row != column) {
    throw "Not a square matrix. Failed to MakeItIdentity().";
  }
  int dimension = row;
  for (int i = 0; i < dimension; ++i) {
    pointer[i][i] = 1.0;
  }
}

void Matrix::MakeItFromSequence(const double *sequence) {
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      pointer[i][j] = sequence[i * column + j];
    }
  }
}

double Matrix::Determinant() {
  if (row != column) {
    throw "Not a square matrix. Failed to Determinant().";
  }
  double result = 1.0;
  int dimension = row;
  Matrix helper{dimension};
  helper.MakeItIdentity();
  for (int i = 0; i < helper.row; ++i) {
    for (int j = 0; j < helper.column; ++j) {
      double temp = 0.0;
      for (int k = 0; k < std::min(i, j); ++k) {
        temp += helper.pointer[i][k] * helper.pointer[k][j];
      }
      if (fabs(helper.pointer[j][j]) < 1e-10) {
        return 0.0;
      }
      helper.pointer[i][j] = (pointer[i][j] - temp) / helper.pointer[j][j];
      if (i == j) {
        result *= helper.pointer[i][j];
      }
    }
  }
  return result;
}

Matrix Matrix::Transpose() {
  Matrix result{column, row};
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < column; ++j) {
      result.pointer[j][i] = pointer[i][j];
    }
  }
  return result;
}

Matrix Matrix::Submatrix(const int &x, const int &y) {
  Matrix helper{row - 1, column - 1};
  for (int i = 0; i < helper.row; ++i) {
    for (int j = 0; j < helper.column; ++j) {
      helper.pointer[i][j] =
          pointer[(i < x) ? i : (i + 1)][(j < y) ? j : (j + 1)];
    }
  }
  return helper;
}

Matrix Matrix::AdjointMatrix() {
  if (row != column) {
    throw "Not a square matrix. Failed to AdjointMatrix().";
  }
  int dimension = row;
  Matrix result{dimension};
  for (int i = 0; i < result.row; ++i) {
    for (int j = 0; j < result.column; ++j) {
      result.pointer[j][i] =
          (((i + j) % 2 == 0) ? 1 : -1) * Submatrix(i, j).Determinant();
    }
  }
  return result;
}

Matrix Matrix::Inverse() {
  if (row != column) {
    throw "Not a square matrix. Failed to Inverse().";
  }
  double determinant = Determinant();
  if (fabs(determinant) < 1e-10) {
    throw "Determinant is zero. Failed to Inverse().";
  }
  return 1 / determinant * AdjointMatrix();
}
