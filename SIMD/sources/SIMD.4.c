/*
* @@name:	SIMD.4c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
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
