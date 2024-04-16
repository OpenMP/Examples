/*
* @@name:	device.5
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.2
*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 128
extern void do_work();

void async_memcpy() {
  int h_dev = omp_get_initial_device();
  int d_dev = omp_get_default_device();
  size_t dsize;
  float h_buf[N];
  void *d_buf;
  int i;

  /* allocate device memory */
  dsize = N * sizeof(float);
  d_buf = omp_target_alloc(dsize, d_dev);
  if (!d_buf)
    abort();

  /* set up host data */
  for (i = 0; i < N; i++) {
    h_buf[i] = i*0.1f;
  }

  /* copy data from host to device asynchronously */
  if (omp_target_memcpy_async(d_buf, h_buf, dsize, 0, 0, 
                              d_dev, h_dev, 0, NULL))
    abort();

  /* do some work here at the same time */
  do_work();

  /* wait for task completion */
  #pragma omp taskwait

  omp_target_free(d_buf, d_dev);
}
