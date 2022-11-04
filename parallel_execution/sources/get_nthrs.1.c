/*
* @@name:	get_nthrs.1
* @@type:	C
* @@operation:	compile
* @@expect:	rt-error
* @@version:	pre_omp_3.0
*/
#include <omp.h>
void work(int i);

void incorrect() {
  int np, i;

  np = omp_get_num_threads();  /* misplaced */

  #pragma omp parallel for schedule(static)
  for (i=0; i < np; i++)
    work(i);
}
