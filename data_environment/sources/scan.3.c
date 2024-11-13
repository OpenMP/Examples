/*
* @@name:	scan.3
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.0
*/
#include <stdio.h>
#define N 100

int main(void)
{
   int c[N], tmp;
   int x = 0;

   // initialization
   for (int k = 0; k < N; k++)
      c[k] = k + 1;

   // c[k] is used for both input and output of scan results
   #pragma omp parallel for simd reduction(inscan,+: x) private(tmp)
   for (int k = 0; k < N; k++) {
      // initialization phase
      tmp = c[k];
      #pragma omp scan init_complete

      // scan (output) phase - cannot use tmp here
      c[k] = x;

      #pragma omp scan exclusive(x)

      // input phase - can use tmp here
      x += tmp;
   }

   printf("x = %d, c[0:3] = %d %d %d\n", x, c[0], c[1], c[2]);
   //           5050,       0  1  3

   return 0;
}
