/*
* @@name:	nthrs_dynamic.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
*/
#include <omp.h>
int main()
{
  omp_set_dynamic(0);
  #pragma omp parallel num_threads(10)
  {
    /* do work here */
  }
  return 0;
}
