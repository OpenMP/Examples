/*
* @@name:	nowait.2c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/

#include <math.h>
void nowait_example2(int n, float *a, float *b, float *c, float *y, float
*z)
{
   int i;
#pragma omp parallel
   {
#pragma omp for schedule(static) nowait
   for (i=0; i<n; i++)
      c[i] = (a[i] + b[i]) / 2.0f;
#pragma omp for schedule(static) nowait
   for (i=0; i<n; i++)
      z[i] = sqrtf(c[i]);
#pragma omp for schedule(static) nowait
   for (i=1; i<=n; i++)
      y[i] = z[i-1] + a[i];
   }
}
