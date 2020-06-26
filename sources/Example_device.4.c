/*
* @@name:	device.6c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.5
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

void get_dev_cos(double *mem, size_t s)
{
   int h, t, i;
   double * mem_dev_cpy;
   h = omp_get_initial_device();
   t = omp_get_default_device();

   if (omp_get_num_devices() < 1 || t < 0){
      printf(" ERROR: No device found.\n");
      exit(1);
   }

   mem_dev_cpy = (double *)omp_target_alloc( sizeof(double) * s, t);
   if(mem_dev_cpy == NULL){
      printf(" ERROR: No space left on device.\n");
      exit(1);
   }

                          /* dst  src */
   omp_target_memcpy(mem_dev_cpy, mem, sizeof(double)*s,
                              0,    0,
                              t,   h);

   #pragma omp target is_device_ptr(mem_dev_cpy) device(t)
   #pragma omp teams distribute parallel for
     for(i=0;i<s;i++){ mem_dev_cpy[i] = cos((double)i); } /* init data */

                   /* dst  src */
    omp_target_memcpy(mem, mem_dev_cpy, sizeof(double)*s,
                      0,             0,
                      h,             t);

    omp_target_free(mem_dev_cpy, t);
}
