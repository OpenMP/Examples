/*
* @@name:	apply_syntax_equivalent.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.1
*/
void equivalent(double A[100])
{
   #pragma omp unroll
   for (int i1 = 0; i1 < 25; ++i1)
   for (int i2 = 0; i2 <  4; ++i2) {
      int i = i1 * 4 + i2;
      A[i] = A[i] + 1;
   }
}
