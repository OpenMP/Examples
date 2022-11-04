/*
* @@name:	allocators.4
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.1
*/
#include <omp.h>
#include <stdio.h>

void my_init(double *,double *,int, double *,double *,int, \
             double *,double *,int);
void lib_saxpy(double *,double *,double,int);
void my_gather(double *,double *,int);

#pragma omp begin declare target
void my_gpu_vxv(double *, double *, int);
#pragma omp end  declare target

#define Nhb 1024*1024      // high bandwith
#define Nbg 1024*1024*64   // big memory, default
#define Nll 1024*1024      // low latency memory

void test_allocate() {

  double  v1[Nhb], v2[Nhb];
  double  v3[Nbg], v4[Nbg];
  double  v5[Nll], v6[Nll];

/*** CASE 1: USING ALLOCATE DIRECTIVE ***/
  #pragma omp allocate(v1,v2) allocator(omp_high_bw_mem_alloc)
  #pragma omp allocate(v3,v4) allocator(omp_default_mem_alloc)  

  my_init(v1,v2,Nhb, v3,v4,Nbg, v5,v6,Nll);

  lib_saxpy(v1,v2,5.0,Nhb);

  #pragma omp target map(to: v3[0:Nbg], v4[0:Nbg]) map(from:v3[0:Nbg])
  my_gpu_vxv(v3,v4,Nbg);

/*** CASE 2: USING ALLOCATE CLAUSE ***/
  #pragma omp task private(v5,v6) \
                   allocate(allocator(omp_low_lat_mem_alloc): v5,v6)
  {
    my_gather(v5,v6,Nll);
  }

}
