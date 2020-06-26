/*
* @@name:	taskloop_simd_reduction.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_5.0
*/

#include <stdio.h>
#define N 100

int main(){
  int i, a[N], asum=0;

  for(i=0;i<N;i++) a[i]=i;

// taskloop reductions

  #pragma omp parallel master 
  #pragma omp taskloop reduction(+:asum)                //taskloop 1
    for(i=0;i<N;i++){ asum += a[i]; }


  #pragma omp parallel reduction(task, +:asum)          // parallel reduction a
  {
     #pragma omp master
     #pragma omp task            in_reduction(+:asum)    //task 2
       for(i=0;i<N;i++){ asum += a[i]; }

     #pragma omp master taskloop in_reduction(+:asum)    //taskloop 2
       for(i=0;i<N;i++){ asum += a[i]; }
  }

// taskloop simd reductions

  #pragma omp parallel master 
  #pragma omp taskloop simd reduction(+:asum)            //taskloop simd 3
    for(i=0;i<N;i++){ asum += a[i]; }


  #pragma omp parallel reduction(task, +:asum)          // parallel reduction b
  {
     #pragma omp master
     #pragma omp task                 in_reduction(+:asum) //task 4
       for(i=0;i<N;i++){ asum += a[i]; }

     #pragma omp master taskloop simd in_reduction(+:asum) //taskloop simd 4
       for(i=0;i<N;i++){ asum += a[i]; }

  }

  printf("asum=%d \n",asum); // output: asum=29700
}
