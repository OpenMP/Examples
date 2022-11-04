/*
* @@name:	acquire_release.1
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.0
*/
#include <stdio.h>
#include <omp.h>

int main()
{
   int x = 0, y = 0;
   #pragma omp parallel num_threads(2)
   {
      int thrd = omp_get_thread_num();
      if (thrd == 0) {
         x = 10;
         #pragma omp critical
         { y = 1; }
      } else {
         int tmp = 0;
         while (tmp == 0) {
           #pragma omp critical
           { tmp = y; }
         }
         printf("x = %d\n", x); // always "x = 10"
      }
   }
   return 0;
}
