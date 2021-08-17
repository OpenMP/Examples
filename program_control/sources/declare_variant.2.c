/*
* @@name: declare_variant.2c
* @@type: C
* @@compilable: yes
* @@linkable: no
* @@expect: success
* @@version: omp_5.0
*/
#include <omp.h>

void   base_saxpy(int, float, float *, float *);
void avx512_saxpy(int, float, float *, float *);

#pragma omp declare variant( avx512_saxpy ) \
                      match( device={isa("core-avx512")} )
void base_saxpy(int n, float s, float *x, float *y)    // base function
{
   #pragma omp parallel for
   for(int i=0; i<n; i++) y[i] = s*x[i] + y[i];
}

void avx512_saxpy(int n, float s, float *x, float *y)    //function variant
{
                                     //assume 64-byte alignment for AVX-512
   #pragma omp parallel for simd simdlen(16) aligned(x,y:64)
   for(int i=0; i<n; i++) y[i] = s*x[i] + y[i];
}

// Above may be in another file scope.                  

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define N 1000

int main()
{
   static float x[N],y[N] __attribute__ ((aligned(64)));
   float s=2.0;
                         // Check for 64-byte aligned
   if( ((intptr_t)y)%64 != 0 || ((intptr_t)x)%64 != 0 )
   { printf("ERROR: x|y not 64-Byte aligned\n"); exit(1); }

   for(int i=0; i<N; i++){ x[i]=i+1; y[i]=i+1; } // initialize

   base_saxpy(N,s,x,y);

   printf("y[0],y[N-1]: %5.0f %5.0f\n",y[0],y[N-1]); //output: y...   3  3000

   return 0;
}
