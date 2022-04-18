/*
* @@name:	linear_modifier.3
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_5.2
*/
#include <stdio.h>

#define N 128

#pragma omp declare simd simdlen(4) uniform(x, y) linear(i:val,step(1))
double func(double x[], double y[], int i)
{
   return (x[i] + y[i]);
}

int main(void)
{
   double x[N], y[N], z1[N], z2;
   int i, k;

   for (i = 0; i < N; i++) {
      x[i] = (double)i;
      y[i] = (double)i*2;
   }

   k = 0;
#pragma omp simd linear(k)
   for (i = 0; i < N; i++) {
      z1[i] = func(x, y, k);
      k++;
   }
 
   for (i = 0; i < N; i++) {
      z2 = (double)(i + i*2);
      if (z1[i] != z2) {
         printf("failed\n");
         return 1;
      }
   }
   printf("passed\n");
   return 0;
}
