/*
* @@name:	SIMD.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_4.0
*/
void star( double *a, double *b, double *c, int n, int *ioff )
{
   int i;
   #pragma omp simd
   for ( i = 0; i < n; i++ )
      a[i] *= b[i] * c[i+ *ioff];
}
