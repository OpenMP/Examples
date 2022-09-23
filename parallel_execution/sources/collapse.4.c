/*
* @@name:	collapse.4
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_5.0
*/
#include <stdio.h>
#define N 20
#define M 10

// routine to calculate a
// For variable a[i]:
// a[i][0],...,a[i][n-1]   contains the n samples
// a[i][n]                 contains the sample mean
// a[i][n+1]               contains the standard deviation 
extern void calc_a(int n,int m, float a[][N+2]);

int main(){
  float a[M][N+2], b[M][M];

  calc_a(N,M,a);

  #pragma omp parallel for collapse(2)
  for (int i = 0; i < M; i++)
     for (int j = i; j < M; j++) 
     {
        float temp = 0.0f;
        for (int k = 0; k < N; k++)
           temp += (a[i][k]-a[i][N])*(a[j][k]-a[j][N]);

        b[i][j] = temp / (a[i][N+1] * a[j][N+1] * (N - 1));
        b[j][i] = b[i][j];
     }

  printf("b[0][0] = %f, b[M-1][M-1] = %f\n", b[0][0], b[M-1][M-1]);

  return 0;
}
