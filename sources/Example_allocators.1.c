/*
* @@name: allocators.1c
* @@type: C
* @@compilable: yes
* @@linkable: yes
* @@expect: success
* @@version: omp_5.0
*/
#include    <omp.h>
#include  <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define N 1000

int main()
{
   float  *x, *y;
   float s=2.0;

omp_memspace_handle_t  xy_memspace = omp_default_mem_space;
omp_alloctrait_t       xy_traits[1]={omp_atk_alignment, 64};
omp_allocator_handle_t xy_alloc    = omp_init_allocator(xy_memspace,1,xy_traits);


   x=(float *)omp_alloc(N*sizeof(float), xy_alloc); 
   y=(float *)omp_alloc(N*sizeof(float), xy_alloc); 

   if( ((intptr_t)(y))%64 != 0 || ((intptr_t)(x))%64 != 0 )
   { printf("ERROR: x|y not 64-Byte aligned\n"); exit(1); }

   #pragma omp parallel
   { 
      #pragma omp for simd simdlen(16) aligned(x,y:64)
      for(int i=0; i<N; i++){ x[i]=i+1; y[i]=i+1; } // initialize

      #pragma omp for simd simdlen(16) aligned(x,y:64)
      for(int i=0; i<N; i++) y[i] = s*x[i] + y[i];
    }


   printf("y[0],y[N-1]: %5.0f %5.0f\n",y[0],y[N-1]); //output: y...   3  3000

   omp_free(x, xy_alloc);
   omp_free(y, xy_alloc);
   omp_destroy_allocator(xy_alloc);

   return 0;
}
