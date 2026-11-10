/*
* @@name:	safesync.1
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.0
*/

#include <stdio.h>
#include <omp.h>

#define NT 16
#define N  100

// Rather than explicitly using the safesync clause below for a non-host
// device, the following requires directive can make the "safesync"
// behavior the default for parallel constructs encountered on a non-host
// device.
// #pragma omp requires device_safesync

int main()
{
   int a[N];
   int b[N];
   int count1 = 0;
   int count2 = 0;
   for (int i = 0; i < N; i++) {
      a[i] = 0;
      b[i] = i;
   }

   #pragma omp target thread_limit(NT) map(to:count1,count2) map(a) map(to:b)
   #pragma omp metadirective \
     when (device={kind(nohost)}: parallel num_threads(NT) safesync) \
     otherwise (parallel num_threads(NT))
   {
      int t, u;
      #pragma omp atomic capture
      t = count1++;
      do {
         #pragma omp atomic read acquire
         u = count2;
      } while (u < t);

      for (int i = 0; i < N; i++) {
         a[i] += b[i];
      }

      #pragma omp atomic release
      count2++;
   }

   for (int i = 0; i < N; i++) {
      if (a[i] != NT*b[i]) {
         printf("\ta[%d] = %d, b[%d] = %d\n",
               i, a[i], i, b[i]);
         return 1;
      }
   }

   return 0;
}
