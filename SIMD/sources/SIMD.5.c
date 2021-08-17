/*
* @@name:	SIMD.5c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.0
*/
void work( double **a, double **b, double **c, int n )
{
   int i, j;
   double tmp;
   #pragma omp for simd collapse(2) private(tmp)
   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
         tmp = a[i][j] + b[i][j];
         c[i][j] = tmp;
      }
   }
}
