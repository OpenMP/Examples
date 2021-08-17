/*
* @@name:	linear_modifier.2cpp
* @@type:	C++
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_4.5
*/
#include <stdio.h>

#define NN 1023
int a[NN];

#pragma omp declare simd linear(ref(p)) linear(uval(i))
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
