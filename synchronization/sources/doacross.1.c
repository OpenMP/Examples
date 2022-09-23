/*
* @@name:	doacross.1
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_5.2
*/

float foo(int i);
float bar(float a, float b);
float baz(float b);

void work( int N, float *A, float *B, float *C )
{
  int i;

  #pragma omp for ordered(1)
  for (i=1; i<N; i++)
  {
    A[i] = foo(i);

  #pragma omp ordered doacross(sink: i-1)
    B[i] = bar(A[i], B[i-1]);
  #pragma omp ordered doacross(source: omp_cur_iteration)

    C[i] = baz(B[i]);
  }
}
