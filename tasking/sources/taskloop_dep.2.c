/*
* @@name:	taskloop_dep.2
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
#include <stdio.h>

void process_work_b(int n, float *B[n])
{
   // Dependences for taskloop iterations in multi-dimensional loop nest

   // TL4 taskloop + collapse
   #pragma omp taskloop collapse(2) nogroup
   for (int i = 1; i < n; i++)
   {
      for (int j = 1; j < n; j++)
      {
         #pragma omp task_iteration depend(inout: B[i][j]) \
                                    depend(in: B[i-1][j], B[i][j-1])
         B[i][j] += B[i][j] * B[i-1][j] * B[i][j-1];
      }
   }

   // T5 other task
   #pragma omp task depend(in: B[n-1][n-1])
   printf("B[n-1][n-1] = %f\n", B[n-1][n-1]);
}
