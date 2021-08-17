/*
* @@name:	linear_modifier.1cpp
* @@type:	C++
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_4.5
*/
#include <stdio.h>
 
#define NN 1023
int a[NN];

#pragma omp declare simd linear(ref(p)) simdlen(8)
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
