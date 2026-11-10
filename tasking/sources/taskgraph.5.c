/*
* @@name:	taskgraph.5
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.0
*/
#include <stdio.h>

#define N 100
#define M 8
#define NG 4

int xi[NG][N];
int yi[NG][N];
int zi[NG][N];
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

void do_parallel_work(const int i)
{
   const int input_index = i % NG;
   int *x = xi[input_index];
   int *y = yi[input_index];
   int *z = zi[input_index];
   #pragma omp parallel masked
      #pragma omp taskgraph graph_id(input_index)
      {
	 // Task A
	 #pragma omp task firstprivate(saved: x) depend(out: *x)
	 get_vals(x, N);
	 // Task B
	 #pragma omp task firstprivate(saved: y) depend(out: *y)
	 get_vals(y, N);
	 // Task C
	 #pragma omp task firstprivate(saved: x,y,z) depend(in: *x,*y)
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
      do_parallel_work(i);
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
