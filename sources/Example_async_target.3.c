/*
* @@name:	async_target.3c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.5
*/

#include <stdio.h>

#define N 1000000      //N must be even
void init(int n, float *v1, float *v2);

int main(){
   int   i, n=N;
   int   chunk=1000;
   float v1[N],v2[N],vxv[N];

   init(n, v1,v2);

   #pragma omp parallel
   {

      #pragma omp master
      #pragma omp target teams distribute parallel for nowait \
                                map(to: v1[0:n/2]) \
                                map(to: v2[0:n/2]) \
                                map(from: vxv[0:n/2]) 
      for(i=0; i<n/2; i++){ vxv[i] = v1[i]*v2[i]; }

      #pragma omp for schedule(dynamic,chunk)
      for(i=n/2; i<n; i++){ vxv[i] = v1[i]*v2[i]; }

   }
   printf(" vxv[0] vxv[n-1] %f %f\n", vxv[0], vxv[n-1]);
   return 0;
}
