/*
* @@name: acquire_release.3.c
* @@type: C
* @@compilable: yes
* @@linkable: yes
* @@expect: success
* @@version: omp_5.0
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
         #pragma omp flush // or with acq_rel or release clause
         #pragma omp atomic write // or with relaxed clause
         y = 1;
      } else {
         int tmp = 0;
         while (tmp == 0) {
           #pragma omp atomic read // or with relaxed clause
           tmp = y;
         }
         #pragma omp flush // or with acq_rel or acquire clause
         printf("x = %d\n", x);  // always "x = 10"
      }
   }
   return 0;
}
