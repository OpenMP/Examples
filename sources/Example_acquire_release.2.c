/*
* @@name: acquire_release.2.c
* @@type: C
* @@compilable: yes, omp_5.0
* @@linkable: yes
* @@expect: success
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
          #pragma omp atomic write release // or seq_cst
          y = 1;
       } else {
          int tmp = 0;
          while (tmp == 0) {
            #pragma omp atomic read acquire // or seq_cst
            tmp = y;
          }
          printf("x = %d\n", x); // always "x = 10"
       }
   }
   return 0;
}
