/*
* @@name:       dyn_groupprivate.1
* @@type:       C++
* @@operation:  run 
* @@expect:     success
* @@version:    omp_6.1
*/
#include <omp.h>
#include <stdio.h>
#define N 100
#define M 5

void init(int *x, int n, int tid)
{
   #pragma omp for
   for (int i = 0; i < n; i++)
     x[i] = tid + i + 1;
}
#pragma omp declare_target enter(init)

void foo(int &sum, int tid, int N, int M)
{
   int *x = (int *) omp_get_dyn_gprivate_ptr(0, omp_access_cgroup);
   int *y = (int *) omp_get_dyn_gprivate_ptr(N*sizeof(int), omp_access_cgroup);
  
   init(x, N, tid);
   init(y, M, tid);
   
   #pragma omp for reduction(+:sum)
   for (int i = 0; i < N; i++) {
     sum += x[i];
   }

   #pragma omp for reduction(+:sum)
   for (int i = 0; i < M; i++) {
     sum += y[i];
   }
}
#pragma omp declare_target enter(foo)

int main()
{
   int sums[4] = {0,0,0,0};

   #pragma omp target teams num_teams(4) thread_limit(100) \
           dyn_groupprivate(N*sizeof(int)+M*sizeof(int))
   #pragma omp parallel
   foo(sums[omp_get_team_num()], omp_get_team_num(), N, M);

   int sum = 0;
   for (int i = 0; i < 4; i++)
      sum += sums[i];

   printf("Result: %d\n", sum);

   return 0;
}
