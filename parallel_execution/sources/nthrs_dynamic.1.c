/*
* @@name:	nthrs_dynamic.1
* @@type:	C
* @@operation:	link
* @@expect:	success
* @@version:	pre_omp_3.0
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
