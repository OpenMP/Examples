/*
* @@name:	init_lock.1
* @@type:	C++
* @@operation:	compile
* @@expect:	success
* @@version:	pre_omp_3.0
*/
#include <omp.h>

omp_lock_t *new_locks() {
  int i;
  omp_lock_t *lock = new omp_lock_t[1000];

  #pragma omp parallel for private(i)
    for (i=0; i<1000; i++)
    { omp_init_lock(&lock[i]); }

    return lock;
}
