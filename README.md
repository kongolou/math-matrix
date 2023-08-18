# Matrix(Math)
Class Matrix written in C++.  
## Public Interfaces
- void MakeItZero()
- void MakeItIdentity()
- void MakeItFromSequence(const double* sequence)
- double Determinant()
- Matrix Transpose()
- Matrix Submatrix(const int& x, const int& y)
- Matrix AdjointMatrix()
- Matrix Inverse()
## Examples
### Test1
```c++
#include "matrix.h"

int main()
{
    Matrix A{2};
    double seq[] = {1., 2., 3., 4.};
    A.MakeFromSequence(seq);
    Matrix B{A * A.Transpose()};
    std::cout << B;
    std::cout << A.Inverse();
}
```
-->
```plaintext 
5.000000	11.000000	
11.000000	25.000000	

-2.000000	1.000000	
1.500000	-0.500000

```
### Test1
```c++
#include "matrix.h"

int main() {
  double seq[] = {1., 2., 3., 4., 5., 6., 7., 8., 9.};
  Matrix mtx{3, 3};
  mtx.MakeItFromSequence(seq);
  std::cout << mtx;
  try {
    mtx.Inverse();
  } catch (const char *s) {
    std::cout << s <<std::endl;
  }
  return 0;
}
```
-->
```plaintext 
Determinant is zero. Failed to Inverse().
```
