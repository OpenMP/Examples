/*
* @@name:	free_agent.3
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.0
*/

// OMP_THREADS_RESERVE="structured(2),free_agent(2)"

#include <stdio.h>
#include <omp.h>

int count = 0;

int main(void) {

   #pragma omp parallel masked num_threads(strict: 2)
     #pragma omp taskloop grainsize(strict: 1) threadset(omp_pool)
       for (int i = 0; i < 40; i++)
         if ( omp_is_free_agent() ) {
          #pragma omp atomic
            count++;
         }
   printf("Number of tasks executed by free-agent threads = %d\n", count);

   return 0;
}
