/*
* @@name:	nthrs_nesting.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
*/
#include <stdio.h>
#include <omp.h>
int main (void)
{
   omp_set_nested(1);
   omp_set_dynamic(0);
   #pragma omp parallel
   {
      #pragma omp parallel
      {
         #pragma omp single
         {
         /*
         * If OMP_NUM_THREADS=2,3 was set, the following should print:
         * Inner: num_thds=3
         * Inner: num_thds=3
         *
         * If nesting is not supported, the following should print:
         * Inner: num_thds=1
         * Inner: num_thds=1
         */
            printf ("Inner: num_thds=%d\n", omp_get_num_threads());
         }
      }
      #pragma omp barrier
      omp_set_nested(0);
      #pragma omp parallel
      {
         #pragma omp single
         {
         /*
         * Even if OMP_NUM_THREADS=2,3 was set, the following should
         * print, because nesting is disabled:
         * Inner: num_thds=1
         * Inner: num_thds=1
         */
            printf ("Inner: num_thds=%d\n", omp_get_num_threads());
         }
      }
      #pragma omp barrier
      #pragma omp single
      {
         /*
         * If OMP_NUM_THREADS=2,3 was set, the following should print:
         * Outer: num_thds=2
         */
         printf ("Outer: num_thds=%d\n", omp_get_num_threads());
      }
   }
   return 0;
}
