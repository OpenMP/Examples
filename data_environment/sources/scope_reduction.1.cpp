/*
* @@name:	scope_reduction.1
* @@type:	C++
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.1
*/
#include <stdio.h>
void do_work(int n, float a[], float &s)
{
   float loc_s = 0.0f;        // local sum
   static int nthrs;
   #pragma omp for
      for (int i = 0; i < n; i++)
         loc_s += a[i];
   #pragma omp single
   {
      s = 0.0f;               // total sum
      nthrs = 0;
   }
   #pragma omp scope reduction(+:s,nthrs)
   {
      s += loc_s;
      nthrs++;
   }
   #pragma omp masked
      printf("total sum = %f, nthrs = %d\n", s, nthrs);
}

float work(int n, float a[])
{
   float s;
   #pragma omp parallel
   {
      do_work(n, a, s);
   }
   return s;
}
