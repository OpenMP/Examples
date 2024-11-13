/*
* @@name:	taskloop_dep.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
#include <stdio.h>

void process_work_a(int n, float *A)
{
   // Dependences for taskloop iterations and across taskloops

   // TL1 taskloop
   // nogroup removes the implicit taskgroup
   #pragma omp taskloop nogroup 
   for (int i = 1; i < n; i++)
   {
      #pragma omp task_iteration depend(inout: A[i]) depend(in: A[i-1])
      A[i] += A[i] * A[i-1];
   }

   // TL2 taskloop + grainsize
   #pragma omp taskloop grainsize(strict: 4) nogroup 
   for (int i = 1; i < n; i++)
   {
      #pragma omp task_iteration depend(inout: A[i]) depend(in: A[i-4]) \
                                 if ((i % 4) == 0 || i == n-1)
      A[i] += A[i] * A[i-1];
   }

   // T3 other task
   #pragma omp task depend(in: A[n-1])
   printf("A[n-1] = %f\n",  A[n-1]);
}
