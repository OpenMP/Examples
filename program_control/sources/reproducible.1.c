/*
* @@name:	reproducible.1
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_5.0
*/
#include <stdio.h>
#include <omp.h>

int main()
{
   const int n = 1000;
   int v[n], u[n];
   static int sum;
   #pragma omp threadprivate(sum)

   // no data dependences, so can execute concurrently
   #pragma omp parallel for order(concurrent)
   for (int i = 0; i < n; i++) {
      u[i] = i;
      v[i] = i;
      v[i] += u[i] * u[i];
   }

   // with data dependences, so cannot execute iterations
   // concurrently with the order(concurrent) clause
   #pragma omp parallel for ordered
   for (int i = 1; i < n; i++) {
      v[i] += u[i] * u[i];
      #pragma omp ordered
	  v[i] += v[i-1];
   }

   sum = 0;
   // accessing a threadprivate variable, which would not be
   // permitted if the order(concurrent) clause was present
   #pragma omp parallel for copyin(sum)
   for (int i = 0; i < n; i++) {
      sum += v[i];
   }

   #pragma omp parallel
   {
      printf("sum = %d on thread %d\n", sum, omp_get_thread_num());
   }

   return 0;
}
