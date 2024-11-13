/*
* @@name:   apply_syntax_equivalent.2
* @@type:   C
* @@operation: compile
* @@expect: success
* @@version:   omp_6.0
*/
void equivalent(double A[100][100][3])
{
  #pragma omp interchange
  for (int i1 = 0; i1 < 25; ++i1)
  #pragma omp nothing
  for (int j1 = 0; j1 < 25; ++j1)

     #pragma omp nothing
     for (int i2 = 0; i2 < 4; ++i2)
     #pragma omp interchange
     for (int j2 = 0; j2 < 4; ++j2)

        for (int k = 0; k < 3; ++k) {
           int i = i1 * 4 + i2;
           int j = j1 * 4 + j2;
           A[i][j][k] = A[i][j][k] + 1;
        }
}
