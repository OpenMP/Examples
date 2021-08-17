/*
* @@name:	nthrs_dynamic.2c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
*/
#include <omp.h>
int main()
{
  omp_set_dynamic(1);
  #pragma omp parallel num_threads(10)
  {
    /* do work here */
  }
  return 0;
}
