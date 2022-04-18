/*
* @@name:	tasking.19
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.0
*/
// Assume BS divides N perfectly
void matmul_depend(int N, int BS, float A[N][N], float B[N][N], float
C[N][N] )
{
   int i, j, k, ii, jj, kk;
   for (i = 0; i < N; i+=BS) {
      for (j = 0; j < N; j+=BS) {
         for (k = 0; k < N; k+=BS) {
#pragma omp task depend ( in: A[i:BS][k:BS], B[k:BS][j:BS] ) \
            depend ( inout: C[i:BS][j:BS] )
            for (ii = i; ii < i+BS; ii++ )
               for (jj = j; jj < j+BS; jj++ )
                  for (kk = k; kk < k+BS; kk++ )
                     C[ii][jj] = C[ii][jj] + A[ii][kk] * B[kk][jj];
         }
      }
   }
}
