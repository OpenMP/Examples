/*
* @@name:	SIMD.4
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_4.0
*/
void work( float *b, int n, int m )
{
   int i;
   #pragma omp simd safelen(16)
   for (i = m; i < n; i++)
      b[i] = b[i-m] - 1.0f;
}
