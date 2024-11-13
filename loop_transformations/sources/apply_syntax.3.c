/*
* @@name:	apply_syntax.3
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
void apply_complexarg(double A[100*100])
{
   #pragma omp tile sizes(4,5) \
      apply(grid(1): reverse)  \
      apply(intratile(2): unroll)
   for (int i = 0; i < 100; ++i)
      for (int j = 0; j < 100; ++j)
         A[i*100+j] += 1;
}
