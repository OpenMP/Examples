/*
* @@name:       target_struct_map.3c
* @@type:       C
* @@compilable: yes
* @@linkable:   yes
* @@expect:     failure
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
    S->p[i] = S->p[i] * S->a + S->b; // S->p[i] invalid
}
#pragma omp end declare target

int main()
{
  struct foo S1, S2;
  int i;

  // Case 1

  S1.a = 2.0;
  S1.b = 4.0;
  S1.p = (float *)malloc(sizeof(float)*N);
  for(i=0; i<N; i++) S1.p[i] = i;

  // No pointer attachment for S1.p here
  #pragma omp target data map(S1.p[:N])
  #pragma omp target // implicit map of S1
  saxpyfun(&S1);

  // Case 2

  S2.a = 2.0;
  S2.b = 4.0;
  S2.p = (float *)malloc(sizeof(float)*N);
  for(i=0; i<N; i++) S2.p[i] = i;

  // No pointer attachment for S2.p here either
  #pragma omp target data map(S2.p[:N])
  #pragma omp target map(S2.p, S2.a, S2.b) // implicit map of S2
  saxpyfun(&S2);

  // These print statement may not execute because the
  // above code is invalid
  printf(" %4.0f %4.0f\n", S1.p[0], S1.p[N-1]);
  printf(" %4.0f %4.0f\n", S2.p[0], S2.p[N-1]);

  free(S1.p);
  free(S2.p);
  return 0;
}
