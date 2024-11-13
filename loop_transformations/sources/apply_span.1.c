/*
* @@name:	apply_span.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
void span_apply(double A[128][128])
{
   #pragma omp for collapse(2)
   #pragma omp tile sizes(16,16) \
               apply(grid: interchange,reverse)
   for (int i = 0; i < 128; ++i)
      for (int j = 0; j < 128; ++j)
         A[i][j] = A[i][j] + 1;
}
