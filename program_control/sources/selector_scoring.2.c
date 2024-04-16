/*
* @@name:	selector_scoring.2
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.0
*/

#include <stdio.h>

// The unified_address and/or unified_shared_memory requirements may be
// implicitly enforced by the implementation via compiler flags.

const int version = 2;

#pragma declare variant(kernel_target_ua) \
   match(implementation={requires(unified_address)})
#pragma declare variant(kernel_target_usm) \
   match(implementation={requires(unified_shared_memory)})
#pragma declare variant(kernel_target_usm_v2) \
   match(implementation={requires(unified_shared_memory)}, \
         user={condition(score(1): version==2)})
void kernel(int *a, int n)
{
   #pragma omp parallel for
   for (int i = 0; i < n; i++) {
      a[i] = i*i;
   }
}

void kernel_target_ua(int *a, int n)
{
   #pragma omp target data map(a[:n]) use_device_ptr(a)
   #pragma omp target parallel for
   for (int i = 0; i < n; i++) {
      a[i] = 2*i*i;
   }
}

void kernel_target_usm(int *a, int n)
{
   #pragma omp target parallel for
   for (int i = 0; i < n; i++) {
      a[i] = 3*i*i;
   }
}

void kernel_target_usm_v2(int *a, int n)
{
   #pragma omp target teams loop
   for (int i = 0; i < n; i++) {
      a[i] = 4*i*i;
   }
}

int main()
{
   int a[1000];   
   
   kernel(a, 1000);
   
   for (int i = 0; i < 1000; i++) {
      if (a[i] != 4*i*i) {
         printf("Failed\n");
         return 1;
      }
   }
   
   printf("Passed\n");
   return 0;
}
