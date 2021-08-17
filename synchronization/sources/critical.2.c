/*
* @@name:	critical.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/
#if _OPENMP  < 201811
#define omp_sync_hint_contended   omp_lock_hint_contended
#define omp_sync_hint_speculative omp_lock_hint_speculative
#endif

#include <omp.h>

int dequeue(float *a);
void work(int i, float *a);

void critical_example(float *x, float *y)
{
  int ix_next, iy_next;

  #pragma omp parallel shared(x, y) private(ix_next, iy_next)
  {
    #pragma omp critical (xaxis) hint(omp_sync_hint_contended)
      ix_next = dequeue(x);
    work(ix_next, x);

    #pragma omp critical (yaxis) hint(omp_sync_hint_contended)
      iy_next = dequeue(y);
    work(iy_next, y);
  }

}
