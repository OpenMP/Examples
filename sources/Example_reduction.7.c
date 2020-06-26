/*
* @@name:	reduction.7c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.5
*/
#include <stdio.h>

#define N 100
void init(int n, float (*b)[N]);

int main(){

  int i,j;
  float a[N], b[N][N];

  init(N,b);

  for(i=0; i<N; i++) a[i]=0.0e0;

  #pragma omp parallel for reduction(+:a[0:N]) private(j)
  for(i=0; i<N; i++){
    for(j=0; j<N; j++){
       a[j] +=  b[i][j];
    }
  }
  printf(" a[0] a[N-1]: %f %f\n", a[0], a[N-1]);

  return 0;
}
