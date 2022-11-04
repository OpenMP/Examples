/*
* @@name:	linear_modifier.2
* @@type:	C++
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.2
*/
#include <stdio.h>

#define NN 1023
int a[NN];

#pragma omp declare simd linear(p: ref) linear(i: uval)
void add_one2(int& p, const int& i)
{
   p += i;
}

int main(void)
{
   int i;
   int* p = a;

   for (i = 0; i < NN; i++) {
      a[i] = i;
   }

   #pragma omp simd linear(p)
   for (i = 0; i < NN; i++) {
      int& k = *p;
      add_one2(k, i);
      p++;
   }

   for (i = 0; i < NN; i++) {
      if (a[i] != i*2) {
         printf("failed\n");
         return 1;
      }
   }
   printf("passed\n");
   return 0;
}
