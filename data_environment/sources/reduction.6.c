/*
* @@name:	reduction.6
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	rt-error
* @@version:    omp_5.1
*/
#include <stdio.h>

int main (void)
{
  int a, i;

  #pragma omp parallel shared(a) private(i)
  {
    #pragma omp masked
    a = 0;

    // To avoid race conditions, add a barrier here.

    #pragma omp for reduction(+:a)
    for (i = 0; i < 10; i++) {
        a += i;
    }

    #pragma omp single
    printf ("Sum is %d\n", a);
  }
  return 0;
}
