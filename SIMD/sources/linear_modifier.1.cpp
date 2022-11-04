/*
* @@name:	linear_modifier.1
* @@type:	C++
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.2
*/
#include <stdio.h>
 
#define NN 1023
int a[NN];

#pragma omp declare simd linear(p: ref) simdlen(8)
void add_one2(int& p)
{
   p += 1;
}

int main(void)
{
   int i;
   int* p = a;

   for (i = 0; i < NN; i++) {
      a[i] = i;
   }

#pragma omp simd linear(p) simdlen(8)
   for (i = 0; i < NN; i++) {
      int& k = *p;
      add_one2(k);
      add_one2(a[i]);
      p++;
   }

   for (i = 0; i < NN; i++) {
      if (a[i] != i+2) {
         printf("failed\n");
         return 1;
      }
   }
   printf("passed\n");
   return 0;
}
