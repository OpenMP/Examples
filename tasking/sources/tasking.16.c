/*
* @@name:	tasking.16
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_4.0
*/
#include <stdio.h>
int main()
{
   int x = 1;
   #pragma omp parallel
   #pragma omp single
   {
      #pragma omp task shared(x) depend(in: x)
         printf("x = %d\n", x);
      #pragma omp task shared(x) depend(out: x)
         x = 2;
   }
   return 0;
}
