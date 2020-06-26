/*
* @@name:       target_struct_map.1c
* @@type:       C
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version:	omp_5.0
*/
#include <stdio.h>
#include <stdlib.h>
#define N 100
#define BAZILLION 2000000

struct foo {
  char buffera[BAZILLION];
  char bufferb[BAZILLION];
  float x;
  float a, b;
  float *p;
};

#pragma omp declare target
void saxpyfun(struct foo *S)
{
  int i;
  for(i=0; i<N; i++) 
    S->p[i] = S->p[i]*S->a + S->b;
}
#pragma omp end declare target

int main()
{
  struct foo S;
  int i;

  S.a = 2.0;
  S.b = 4.0;
  S.p = (float *)malloc(sizeof(float)*N);
  for(i=0; i<N; i++) S.p[i] = i;

  #pragma omp target map(alloc:S.p) map(S.p[:N]) map(to:S.a, S.b)
  saxpyfun(&S);

  printf(" %4.0f %4.0f\n", S.p[0], S.p[N-1]);
        //     4  202  <- output
  return 0;
}
