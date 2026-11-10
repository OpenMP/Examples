/*
* @@name:	taskgraph.3
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.0
*/
#include <stdio.h>

#define N 100
#define M 8

int sums[M]={0};

void get_vals(int b[], const int n)
{
   for (int j = 0; j < n; j++) {
      b[j] = j;
   }
}

void do_compute(int z[], int x[], int y[], const int n)
{
   for (int j = 0; j < n; j++) {
      z[j] = x[j] * y[j] - x[j];
   }
}

void exec_taskgraph(const int i)
{
   int xa[N];
   int ya[N];
   int za[N];
   int *x = xa;
   int *y = ya;
   int *z = za;
   #pragma omp taskgraph
   {
      // Task A
      #pragma omp task depend(out: *x)
      get_vals(x, N);
      // Task B
      #pragma omp task depend(out: *y)
      get_vals(y, N);
      // Task C
      #pragma omp task depend(in: *x,*y)
      {
	do_compute(z, x, y, N);
	#pragma omp simd reduction(+:sums[i])
	for (int j = 0; j < N; j++)
	  sums[i] += z[j];
      }
   }
}

int check_sum(const int n)
{
   return  ( (n-1) * n * (2*n-4) ) / 6;
}

int main()
{
   for (int i = 0; i < M; i++) {
      #pragma omp parallel masked
      exec_taskgraph(i);
   }
   for (int i = 0; i < M; i++) {
      if (sums[i] != check_sum(N)) {
         printf("check failed\n");
         return 1;
      }
   }
   printf("check passed\n");
   return 0;
}
