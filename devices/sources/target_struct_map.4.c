/*
* @@name:       target_ptr_map.4.c
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
  struct foo S1, S2, S3;
  int i;

  // Case 1

  S1.a = 2.0;
  S1.b = 4.0;
  S1.p = (float *)malloc(sizeof(float)*N);
  for(i=0; i<N; i++) S1.p[i] = i;

  // The target data construct results in pointer attachment for S1.p.
  // Explicitly mapping S1.p, S1.a, and S1.b rather than S1 avoids mapping the
  // entire structure (including members buffera, bufferb, and x).
  #pragma omp target data map(S1.p[:N],S1.p,S1.a,S1.b)
  #pragma omp target //implicit map of S1
  saxpyfun(&S1);


  // Case 2

  S2.a = 2.0;
  S2.b = 4.0;
  S2.p = (float *)malloc(sizeof(float)*N);
  for(i=0; i<N; i++) S2.p[i] = i;

  // The target construct results in pointer attachment for S2.p.
  #pragma omp target data map(S2.p[:N])
  #pragma omp target map(S2.p[:0], S2.a, S2.b) // implicit map of S2
  saxpyfun(&S2);

  // Case 3

  S3.a = 2.0;
  S3.b = 4.0;
  S3.p = (float *)malloc(sizeof(float)*N);
  for(i=0; i<N; i++) S3.p[i] = i;

  // The target construct results in pointer attachment for S3.p.
  // Note that S3.p is implicitly mapped due to the implicit map of S3 (but
  // corresponding storage is NOT created for members buffera, bufferb, and x).
  #pragma omp target map(S3.p[:N], S3.a, S3.b)  // implicit map of S3
  saxpyfun(&S3);

  printf(" %4.0f %4.0f\n", S1.p[0], S1.p[N-1]);  //OUT1 4 202
  printf(" %4.0f %4.0f\n", S2.p[0], S2.p[N-1]);  //OUT2 4 202
  printf(" %4.0f %4.0f\n", S3.p[0], S3.p[N-1]);  //OUT3 4 202

  free(S1.p);
  free(S2.p);
  free(S3.p);
  return 0;
}

