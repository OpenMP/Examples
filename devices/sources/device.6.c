/*
* @@name:	device.5
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.2
*/
#include <stdlib.h>
#include <omp.h>
extern void do_work(int, float *);
extern void do_more_work(int, float *);
#pragma omp declare target enter(do_more_work)

void async_work(int N, float *d_buf, float *h_buf)
{
   omp_depend_t obj;
   int d_dev, h_dev;
   size_t dsize;

   h_dev = omp_get_initial_device();
   d_dev = omp_get_default_device();
   dsize = N * sizeof(float);

   // initialize a depend object 'obj'
   #pragma omp depobj(obj) depend(out: d_buf[0:N])

   // start the async memcpy of s_buf to d_buf on device
   if (omp_target_memcpy_async(d_buf, h_buf, dsize, 0, 0, 
                               d_dev, h_dev, 1, &obj))
      abort();

   // do some useful work at the same time on host
   do_work(N, h_buf);

   // wait until memcpy finishes before using d_buf in the target region
   #pragma omp target is_device_ptr(d_buf) depend(depobj: obj)
   do_more_work(N, d_buf);
}
