/*
* @@name: target_reduction.2.c
* @@type: C
* @@compilable: yes
* @@linkable: yes
* @@expect: success
* @@version: omp_5.0
*/
#include <stdio.h>
int f(int);
int g(int);
int main()
{
   int sum1=0, sum2=0;
   int i;
   const int n = 100;

   #pragma omp target data map(sum1,sum2)
   {
      #pragma omp target teams distribute reduction(+:sum1)
      for (int i = 0; i < n; i++) {
         sum1 += f(i);
      }

      #pragma omp target teams distribute map(sum1) reduction(+:sum2)
      for (int i = 0; i < n; i++) {
         sum2 += g(i) * sum1;
      }
   }
   printf(  "sum1 = %d, sum2 = %d\n", sum1, sum2);
   //OUTPUT: sum1 = 9900, sum2 = 147015000
   return 0;
}

int f(int res){ return res*2; }
int g(int res){ return res*3; }
