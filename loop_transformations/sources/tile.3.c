/*
* @@name:       tile.3
* @@type:       C   
* @@operation:  compile
* @@expect:     success
* @@version:    omp_6.0
*/
#include <stdio.h>

#define M 9
#define N 7
int main() {
  int  A[M][N];
  #pragma omp parallel for collapse(2) schedule(static) num_threads(4)
  #pragma omp tile sizes(grid: 2, 2)
  for (int i = 1; i <= M; ++i)
  for (int j = 1; j <= N; ++j)
    A[i-1][j-1] = i*100 + j;

  for (int i = M; i >= 1; --i)
  {
    for (int j = 1; j <= N; ++j) 
      printf( "%8d", A[i-1][j-1]);
    printf("\n") ;
  }

return 0;
}
