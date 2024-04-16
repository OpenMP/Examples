/*
* @@name:	selector_scoring.1
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.0
*/

#include <stdlib.h>
#include <stdio.h>

#pragma omp declare variant(fx1) match(construct={target})
#pragma omp declare variant(fx2) match(construct={teams,parallel,for})
#pragma omp declare variant(fx3) match(device={kind(gpu),isa(sm_70)})
#pragma omp declare variant(fx4) match(device={arch(nvptx),isa(sm_70)})
void f(int *a, int i)
{
  *a = i;
}

void fx1(int *a, int i)
{
  *a = i;
}

void fx2(int *a, int i)
{
   *a = 2*i;
}

void fx3(int *a, int i)
{
   *a = 3*i;
}

void fx4(int *a, int i)
{
   *a = 4*i;
}

int main()
{
   #define N 4
   int a[N];
   #pragma omp target teams distribute parallel for map(a[:N])
   for (int i = 0; i < N; i++) {
      #pragma omp task
      {
         f(&a[i], i);
      }
   }
   
   for (int i = 0; i < N; i++) {
      if (a[i] != 4*i) {
         printf("Failed\n");
         return 1;
      }
   }

   printf("Passed\n");
   return 0;
}
