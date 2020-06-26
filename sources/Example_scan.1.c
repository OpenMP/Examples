/*
* @@name:       scan.1.c
* @@type:       C
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version: omp_5.0
*/
#include <stdio.h>
#define N 100

int main(void)
{
   int a[N], b[N];
   int x = 0;

   // initialization
   for (int k = 0; k < N; k++)
      a[k] = k + 1;

   // a[k] is included in the computation of producing results in b[k]
   #pragma omp parallel for simd reduction(inscan,+: x)
   for (int k = 0; k < N; k++) {
      x += a[k];
      #pragma omp scan inclusive(x)
      b[k] = x;
   }

   printf("x = %d, b[0:3] = %d %d %d\n", x, b[0], b[1], b[2]);
   //           5050,        1  3  6

   return 0;
}
