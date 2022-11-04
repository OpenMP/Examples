/*
* @@name:	task_dep.3
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_4.0
*/
#include <stdio.h>
int main() {
   int x;
   #pragma omp parallel
   #pragma omp single
   {
      #pragma omp task shared(x) depend(out: x)
         x = 1;
      #pragma omp task shared(x) depend(out: x)
         x = 2;
      #pragma omp taskwait
      printf("x = %d\n", x);
   }
   return 0;
}
