/*
* @@name:	SIMD.2c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_4.0
*/
#include <stdio.h>

#pragma omp declare simd uniform(fact)
double add1(double a, double b, double fact)
{
   double c;
   c = a + b + fact;
   return c;
}

#pragma omp declare simd uniform(a,b,fact) linear(i:1)
double add2(double *a, double *b, int i, double fact)
{
   double c;
   c = a[i] + b[i] + fact;
   return c;
}

#pragma omp declare simd uniform(fact) linear(a,b:1)
double add3(double *a, double *b, double fact)
{
   double c;
   c = *a + *b + fact;
   return c;
}

void work( double *a, double *b, int n )
{
   int i;
   double tmp;
   #pragma omp simd private(tmp)
   for ( i = 0; i < n; i++ ) {
      tmp  = add1( a[i],  b[i], 1.0);
      a[i] = add2( a,     b, i, 1.0) + tmp;
      a[i] = add3(&a[i], &b[i], 1.0);
   }
}

int main(){
   int i;
   const int N=32;
   double a[N], b[N];
 
   for ( i=0; i<N; i++ ) {
      a[i] = i; b[i] = N-i;
   }
 
   work(a, b, N );
 
   for ( i=0; i<N; i++ ) {
      printf("%d %f\n", i, a[i]);
   }

   return 0;
}
