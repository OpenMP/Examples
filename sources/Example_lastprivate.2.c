/*
* @@name:	lastprivate.2c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version: omp_5.0
*/
#include <math.h>

float condlastprivate(float *a, int n)
{
   float x = 0.0f;

   #pragma omp parallel for simd lastprivate(conditional: x)
   for (int k = 0; k < n; k++) {
      if (a[k] < 108.5 || a[k] > 208.5) {
         x = sinf(a[k]);
      }
   }

   return x;
}
