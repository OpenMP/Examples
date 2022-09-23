/*
* @@name:	unroll.1
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:    omp_5.1
*/
void unroll(double A[], int n)
{
   #pragma omp unroll
   for (int i = 0; i < n; ++i)
      A[i] = 0;
}

void unroll_full(double A[])
{
   #pragma omp unroll full
   for (int i = 0; i < 4; ++i)
      A[i] = 0;
}

void unroll_full_equivalent(double A[])
{
   A[0] = 0;
   A[1] = 0;
   A[2] = 0;
   A[3] = 0;
}

