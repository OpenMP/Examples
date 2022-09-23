/*
* @@name:	parallel_masked_taskloop.1
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_5.1
*/
#include <stdio.h>
#define N 100

int main()
{
   int i, a[N],b[N],c[N];

   for(int i=0; i<N; i++){ b[i]=i; c[i]=i; }  //init

   #pragma omp parallel 
   #pragma omp masked
   #pragma omp taskloop                      // taskloop 1
   for(i=0;i<N;i++){ a[i] = b[i] + c[i]; } 

   #pragma omp parallel masked taskloop      // taskloop 2
   for(i=0;i<N;i++){ b[i] = a[i] + c[i]; } 

   #pragma omp parallel masked taskloop simd // taskloop 3
   for(i=0;i<N;i++){ c[i] = a[i] + b[i]; } 

   printf(" %d %d\n",c[0],c[N-1]);  // 0 and 495
}
