/*
* @@name:	unroll.3
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:    omp_5.1
*/
void unroll3_partial(double A[])
{
   #pragma omp unroll partial(4)
   for (int i = 0; i < 128; ++i)
       A[i] = 0;
}

void unroll3_partial_equivalent(double A[])
{
   for (int i_iv = 0; i_iv < 32; ++i_iv) {
       A[i_iv * 4 + 0] = 0;
       A[i_iv * 4 + 1] = 0;
       A[i_iv * 4 + 2] = 0;
       A[i_iv * 4 + 3] = 0;
  }
}

void unroll3_partial_nofactor(double A[])
{
   #pragma omp unroll partial
   for (int i = 0; i < 128; ++i)
       A[i] = 0;
}
