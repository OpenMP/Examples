/*
* @@name:	declare_variant.3
* @@type:	C
* @@operation:	view
* @@expect:	none
* @@version:	omp_5.1
*/

#include <omp.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _OPENMP
#pragma omp begin declare variant match(device={kind(nohost)})

  #pragma omp begin declare variant match(implementation={vendor(nvidia)})

    #pragma omp begin declare variant match(device={isa(sm_70)})
      #include "sm70/my_cuda_fun.h"   /* only included if isa is sm70 */
    #pragma omp end declare variant

    #pragma omp begin declare variant match(device={isa(sm_80)})
      #include "sm80/my_cuda_fun.h"   /* only included if isa is sm80 */
    #pragma omp end declare variant

  #pragma omp end declare variant

  #pragma omp begin declare variant match(implementation={vendor(amd)})
    #include "amdgpu/my_hip_fun.h" /* only included for AMD */
  #pragma omp end declare variant

#pragma omp end declare variant

#pragma omp begin declare variant match(device={kind(host)})
  #include "openmp_host/my_fun.h" 
#pragma omp end declare variant   
#else
    #include "generic/my_fun.h"
#endif

#define N 64
double array[N];
#pragma omp target 

int main() {
   // Array initialization
  for (int i = 0; i < N; ++i) {
    array[i] = 0.0;
  }

#pragma omp target map(tofrom: array[0:N]) 
  for (int i = 0; i < N; ++i) {
    array[i] = my_fun(i);
  }
  return 0;

}
