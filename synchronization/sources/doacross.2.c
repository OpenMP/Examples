/*
* @@name:	doacross.2
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_5.2
*/

float foo(int i, int j);
float bar(float a, float b, float c);
float baz(float b);

void work( int N, int M, float **A, float **B, float **C )
{
  int i, j;

  #pragma omp for ordered(2)
  for (i=1; i<N; i++)
  {
    for (j=1; j<M; j++)
    {
      A[i][j] = foo(i, j);

  #pragma omp ordered doacross(sink: i-1,j) doacross(sink: i,j-1)
      B[i][j] = bar(A[i][j], B[i-1][j], B[i][j-1]);
  #pragma omp ordered doacross(source:)

      C[i][j] = baz(B[i][j]);
    }
  }
}
