/*
* @@name:	apply_syntax.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
void construct_unroll(double A[100])
{
   #pragma omp unroll
   #pragma omp tile sizes(4)
   for (int i = 0; i < 100; ++i)
      A[i] = A[i] + 1;
}

void apply_unroll(double A[100])
{
   #pragma omp tile sizes(4) apply(grid: unroll)
   for (int i = 0; i < 100; ++i)
      A[i] = A[i] + 1;
}
