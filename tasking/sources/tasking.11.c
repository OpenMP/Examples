/*
* @@name:	tasking.11
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_3.1
*/
#include <stdio.h>
void foo ( )
{
   int x = 2;
   #pragma omp task shared(x) mergeable
   {
      x++;
   }
   #pragma omp taskwait
   printf("%d\n",x);  // prints 3
}
