/*
* @@name:	tasking.11c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
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
