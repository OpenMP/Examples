/*
* @@name:	SIMD.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/
void star( double *a, double *b, double *c, int n, int *ioff )
{
   int i;
   #pragma omp simd
   for ( i = 0; i < n; i++ )
      a[i] *= b[i] * c[i+ *ioff];
}
