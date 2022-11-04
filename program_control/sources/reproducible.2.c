/*
* @@name:	reproducible.2
* @@type:	C
* @@operation:	link
* @@expect:	success
* @@version:	omp_5.1
*/
#include <stdio.h>

int main()
{
   const int n = 1000;
   int v[n], u[n];

   #pragma omp parallel 
   {
      // reproducible schedules are used for the following two constructs
      #pragma omp for order(reproducible: concurrent) nowait
      for (int i = 0; i < n; i++) {
         u[i] = i;
         v[i] = i;
      }
      #pragma omp for order(reproducible: concurrent)
      for (int i = 0; i < n; i++) {
         v[i]  += u[i] * u[i];
      }	  
   }

   #pragma omp parallel 
   {
      // static schedules preserve data dependences between the loops
      #pragma omp for schedule(static) order(concurrent) nowait
      for (int i = 0; i < n; i++) {
         u[i] = i;
         v[i] = i;
      }
      #pragma omp for schedule(static) order(concurrent)
      for (int i = 0; i < n; i++) {
         v[i]  += u[i] * u[i];
      }	  
   }

   #pragma omp parallel 
   {
      // the default reproducibility by the static schedule is not 
      // preserved due to the unconstrained order clause.
      // use of nowait here could result in data race.
      #pragma omp for schedule(static) order(unconstrained: concurrent)
      for (int i = 0; i < n; i++) {
         u[i] = i;
         v[i] = i;
      }
      #pragma omp for schedule(static) order(unconstrained: concurrent)
      for (int i = 0; i < n; i++) {
         v[i]  += u[i] * u[i];
      }	  
   }

   return 0;
}
