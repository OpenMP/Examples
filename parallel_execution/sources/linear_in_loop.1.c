/*
* @@name:	linear_in_loop.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_4.5
*/
#include <stdio.h>

#define N 100
int main(void)
{
   float a[N], b[N/2];
   int i, j;

   for ( i = 0; i < N; i++ )
      a[i] = i + 1;

   j = 0;
   #pragma omp parallel
   #pragma omp for linear(j:1)
   for ( i = 0; i < N; i += 2 ) {
      b[j] = a[i] * 2.0f;
      j++;
   }

   printf( "%d %f %f\n", j, b[0], b[j-1] );
   /* print out: 50 2.0 198.0 */

   return 0;
}
