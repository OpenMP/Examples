/*
* @@name:	device.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.0
*/
#include <stdio.h>
#include <omp.h>

#pragma omp declare target
void vec_mult(float *p, float *v1, float *v2, int N);
extern float *p, *v1, *v2;
extern int N;
#pragma omp end declare target

extern void init_vars(float *, float *, int);
extern void output(float *, int);

void foo()
{
   init_vars(v1, v2, N);
   #pragma omp target device(42) map(p[:N], v1[:N], v2[:N])
   {
      vec_mult(p, v1, v2, N);
   }
   output(p, N);
}

void vec_mult(float *p, float *v1, float *v2, int N)
{
   int i;
   int nthreads;
   if (!omp_is_initial_device())
   {
      printf("1024 threads on target device\n");
      nthreads = 1024;
   }
   else
   {
      printf("8 threads on initial device\n");
      nthreads = 8;
   }
   #pragma omp parallel for private(i) num_threads(nthreads)
   for (i=0; i<N; i++)
     p[i] = v1[i] * v2[i];
}
