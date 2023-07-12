# Matrix
Class Matrix written in C++.  
There is no try-catch blocks.  
Just keep rules in heart. : )  
## Public Interface
- void MakeZero()
- void MakeIdentity()
- void MakeFromSequence(const double* sequence)
- double Determinant()
- double Cofactor(const int& x, const int& y)
- Matrix Transpose()
- Matrix AdjointMatrix()
- Matrix Inverse()
## Example
main.cpp
```c++
#include "matrix.h"

int main()
{
    Matrix A{2};
    double seq[]{1., 2., 3., 4.};
    A.MakeFromSequence(seq);
    Matrix B{A * A.Transpose()};
    std::cout << B;
    std::cout << A.Inverse();
}
```
output:
```plaintext 
5.000000	11.000000	
11.000000	25.000000	

-2.000000	1.000000	
1.500000	-0.500000

```
