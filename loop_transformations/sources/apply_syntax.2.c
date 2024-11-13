/*
* @@name:   apply_syntax.2
* @@type:   C
* @@operation: compile
* @@expect:	success
* @@version:	omp_6.0
*/
void apply_assoc(double A[100][100][3])
{
   #pragma omp tile sizes(4,4) \
               apply(     grid: interchange,nothing) \
               apply(intratile: nothing,interchange)
   for (int i = 0; i < 100; ++i)
   for (int j = 0; j < 100; ++j)

      // k loop not associated with tile, but with interchange
      for (int k = 0; k < 3; ++k)
         A[i][j][k] = A[i][j][k] + 1;
}
