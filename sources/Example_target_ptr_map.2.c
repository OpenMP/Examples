/*
* @@name:       target_ptr_map.2c
* @@type:       C
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version:	omp_5.0
*/
#include <stdio.h>
#include <stdlib.h>
#define N 100

#pragma omp declare target
int *p;
extern void use_arg_p(int *p, int n);
extern void use_global_p(     int n);
#pragma omp end declare target

int main()
{
  int i;
  p = (int *)malloc(sizeof(int)*N);
 
  #pragma omp target map(p[:N])  // device p attached to array section
  {
    for (i=0; i<N; i++) p[i] = i;
    use_arg_p(p, N);
    use_global_p(N);
  }                              // value of host p is preserved

  printf(" %3.3d %3.3d\n", p[1], p[N-1]); 
         // 003   297   <- output

  free(p);
  return 0;
}

//#pragma omp declare target (optional here because of prototype spec)
void use_arg_p(int *q, int n)
{
  int i;
  for (i=0; i<n; i++)
    q[i] *= 2;
}

void use_global_p(int n)
{
  int i;
  for (i=0; i<n; i++)
    p[i] += i;   // valid since p is in declare target and called from
                 // inside target region where p was attached to valid memory
}
//#pragma omp end declare target (optional here because of prototype spec)
