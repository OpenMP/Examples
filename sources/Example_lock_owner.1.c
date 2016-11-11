/*
* @@name:	lock_owner.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
*/
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main()
{
  int x;
  omp_lock_t lck;

  omp_init_lock (&lck);
  omp_set_lock (&lck);
  x = 0;

#pragma omp parallel shared (x)
  {
    #pragma omp master
      {
        x = x + 1;
        omp_unset_lock (&lck);
      }

    /* Some more stuff. */
  }
  omp_destroy_lock (&lck);
  return 0;
}
