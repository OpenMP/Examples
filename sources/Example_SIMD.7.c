/*
* @@name:	SIMD.7c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_4.0
*/
#include <stdio.h>
#include <stdlib.h>
 
#define N 45
int a[N], b[N], c[N];

#pragma omp declare simd inbranch
int fib( int n )
{
   if (n <= 1)
      return n;
   else {
      return fib(n-1) + fib(n-2);
   }
}
 
int main(void)
{
   int i;

   #pragma omp simd
   for (i=0; i < N; i++) b[i] = i;

   #pragma omp simd
   for (i=0; i < N; i++) {
      a[i] = fib(b[i]);
   }
   printf("Done a[%d] = %d\n", N-1, a[N-1]);
   return 0;
}
