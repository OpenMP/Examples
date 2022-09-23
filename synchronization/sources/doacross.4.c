/*
* @@name:	doacross.4
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_5.2
*/

double foo(int i, int j);

void work( int N, int M, double **A, double **B, double **C )
{
  int i, j;
  double alpha = 1.2;

  #pragma omp for collapse(2) ordered(2)
  for (i = 1; i < N-1; i++)
  {
    for (j = 1; j < M-1; j++)
    {
      A[i][j] = foo(i, j);
  #pragma omp ordered doacross(source:)

      B[i][j] = alpha * A[i][j];

  #pragma omp ordered doacross(sink: i-1,j) doacross(sink: i,j-1)
      C[i][j] = 0.2 * (A[i-1][j] + A[i+1][j] +
                A[i][j-1] + A[i][j+1] + A[i][j]);
    }
  }
}
