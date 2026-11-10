/*
* @@name:	free_agent.2
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.0
*/

// OMP_THREADS_RESERVE="structured(64),free_agent(4)"

#include <stdio.h>
#include <omp.h>

int count = 0;
int fib(int n) {
   int i, j;

   if ( n<2 )
      return n;

   #pragma omp task shared(i) threadset(omp_pool)
      i=fib(n-1);
   #pragma omp task shared(j) threadset(omp_pool)
      j=fib(n-2);
   #pragma omp taskwait
   if ( omp_is_free_agent() ) {
      #pragma omp atomic
      count++;
   }
   return (i+j); 
}


int main(void) {
   int val;

   val = fib(20);
   printf("fib(20) = %d\n", val);
   printf("Number of tasks executed by free-agent threads = %d\n", count);

   return 0;
}
