/*
* @@name:	task_dep.16
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.1
*/
#include <assert.h>

int main()
{
   int x, y;
   
   // Case 1: T0 completes before T2 due to task dependence on x
   y = 0;
   #pragma omp parallel masked
   {
      #pragma omp task transparent(omp_export) depend(out:x) // T0
      {
         #pragma omp task depend(out:y) // T1
         {
            y = 1;
         }
      }
      #pragma omp task depend(in: x, y) // T2
      {
         assert(y == 1);
      }
   }
   
   // Case 2: U0 completes before U2 due to being undeferred
   y = 0;
   #pragma omp parallel masked
   {
      #pragma omp task transparent(omp_export) if(0) // U0
      {
         #pragma omp task depend(out:y) // U1
         {
            y = 1;
         }
      }
      #pragma omp task depend(in: y) // U2
      {
         assert(y == 1);
      }
   }

   // Case 3: V0 completes before V2 due to taskwait
   y = 0;
   #pragma omp parallel masked
   {
      #pragma omp task transparent(omp_export) // V0
      {
         #pragma omp task depend(out:y) // V1
         {
            y = 1;
         }
      }
      #pragma omp taskwait
      #pragma omp task depend(in: y) // V2
      {
         assert(y == 1);
      }
   }

   return 0;
}