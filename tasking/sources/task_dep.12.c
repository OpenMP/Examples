/*
* @@name:	task_dep.12c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_4.0
*/
#include <stdio.h>
int main (int argc, char *argv[])
{
  int x = 0;
  #pragma omp parallel
  #pragma omp single
  {
    /* first explicit task */
    #pragma omp task shared(x) depend(out: x)
      x = 1;

    /* second explicit task */
    #pragma omp task shared(x) depend(inout: x) if(0)
      x = 2;

    /* statement executed by parent implicit task
       prints: x = 2 */
    printf("x = %d\n", x);
  }
  return 0;
}
