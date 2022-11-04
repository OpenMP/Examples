/*
* @@name:	init_lock_with_hint.1
* @@type:	C++
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.0
*/
#include <omp.h>

omp_lock_t *new_locks()
{
  int i;
  omp_lock_t *lock = new omp_lock_t[1000];

  #pragma omp parallel for private(i)
    for (i=0; i<1000; i++)
    {
      omp_init_lock_with_hint(&lock[i], 
        static_cast<omp_lock_hint_t>(omp_sync_hint_contended |
                                     omp_sync_hint_speculative));
    }
    return lock;
}
