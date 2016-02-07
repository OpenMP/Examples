/*
* @@name:	critical.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/
#include <omp.h>

int dequeue(float *a);
void work(int i, float *a);

void critical_example(float *x, float *y)
{
  int ix_next, iy_next;

  #pragma omp parallel shared(x, y) private(ix_next, iy_next)
  {
    #pragma omp critical (xaxis) hint(omp_lock_hint_contended)
      ix_next = dequeue(x);
    work(ix_next, x);

    #pragma omp critical (yaxis) hint(omp_lock_hint_contended)
      iy_next = dequeue(y);
    work(iy_next, y);
  }

}
