/*
* @@name:	priv_reduction.3
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.0
*/
#include <stdio.h>

int main(void)
{
  int x;

  x = 0;
  // parallel reduction on shared x
  #pragma omp parallel reduction(+: x) num_threads(strict: 4)
  {
    #pragma omp for reduction(+: x)   // reduction on private x
      for (int i = 0; i < 10; i++)
        x++;
  }
  printf("x = %d\n", x);    // should print 40, with 4 threads
  return 0;
}
