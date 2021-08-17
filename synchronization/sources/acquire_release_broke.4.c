/*
* @@name: acquire_release.4.c
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

// !!! THIS CODE WILL FAIL TO PRODUCE CONSISTENT RESULTS !!!!!!!
// !!! DO NOT PROGRAM SYNCHRONIZATION THIS WAY !!!!!!!

   int x = 0, y;
   #pragma omp parallel num_threads(2)
   {
      int thrd = omp_get_thread_num();
      if (thrd == 0) {
         #pragma omp critical
         { x = 10; }
         // an explicit flush directive that provides
         // release semantics is needed here
         // to complete the synchronization.
         #pragma omp atomic write
         y = 1;
      } else {
         int tmp = 0;
         while (tmp == 0) {
            #pragma omp atomic read acquire // or seq_cst
            tmp = y;
         }
         #pragma omp critical
         { printf("x = %d\n", x); }  // !! NOT ALWAYS 10
      }
   }
   return 0;
}
