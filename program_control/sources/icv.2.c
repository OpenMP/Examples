/*
* @@name:	icv.2
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.0
* @@env:	OMP_NUM_THREADS="4,5,6"
* @@env:	OMP_MAX_ACTIVE_LEVELS=3
*/
#include <stdio.h>
#include <omp.h>

void prn_info(int level)
{
   #pragma omp masked
      printf("LV%d: nthrs_next=%d\n",
             level, omp_get_max_threads());
}

// run with OMP_NUM_THREADS="4,5,6" OMP_MAX_ACTIVE_LEVELS=3
int main (void)
{
   // nthreads-var: 4,5,6
   // max-active-levels-var: 3

   // Case 1
   #pragma omp parallel // request 4 threads
   {
      prn_info(1);      // LV1: nthrs_next=5

      // nthreads-var: 5,6
      #pragma omp parallel // request 5 threads
      {
         prn_info(2);      // LV2: nthrs_next=6

         // nthreads-var: 6
         #pragma omp parallel // request 6 threads
         {
            prn_info(3);      // LV3: nthrs_next=6

            // nthreads-var: 6
         }
      }
   }

   // Case 2
   #pragma omp parallel num_threads(8)
   {
      prn_info(1);      // LV1: nthrs_next=5

      // nthreads-var: 5,6
      #pragma omp parallel // request 5 threads
      {
         prn_info(2);      // LV2: nthrs_next=6

         // nthreads-var: 6
         #pragma omp parallel // request 6 threads
         {
            prn_info(3);      // LV3: nthrs_next=6

            // nthreads-var: 6
         }
      }
   }

   // Case 3
   #pragma omp parallel num_threads(8,2)
   {
      prn_info(1);      // LV1: nthrs_next=2

      // nthreads-var: 2
      #pragma omp parallel // request 2 threads
      {
         prn_info(2);      // LV2: nthrs_next=2

         // nthreads-var: 2
         #pragma omp parallel // request 2 threads
         {
            prn_info(3);      // LV3: nthrs_next=2

            // nthreads-var: 2
         }
      }
   }

   return 0;
}
