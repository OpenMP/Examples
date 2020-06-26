/*
* @@name:       target_struct_map.2c
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
int a;
#pragma omp end declare target

int main(){
  int i;
  int *p;

  #pragma omp target data map(p)
  {
    p = (int *)malloc(sizeof(int)*N);
    for (i=0; i<N; i++) p[i] = i;
    a = 1;

    #pragma omp target map(p[:N]) // var a -- effective map(alloc:a)
    {                             // here and is persistent
      a=2;
      for (i=0; i<N; i++) p[i] *= a;
      a=3;
    }

    printf("a=%d, p[1],p[N-1]: %d %d\n", a, p[1], p[N-1]);
         //    1     2    198 

    #pragma omp target map(p[:N]) // a is persistent
    for (i=0; i<N; i++) p[i] += a;

    printf("a=%d, p[1],p[N-1]: %d %d\n", a, p[1], p[N-1]);
         //    1     5    201 
  }
  return 0;
}
