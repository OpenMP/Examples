/*
* @@name:	apply_nested_equivalent.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
void step1(double A[100])
{
   for (int i1 = 0; i1 < 10; ++i1)
   #pragma omp unroll partial(2) apply(reverse)
   for (int i2 = 0; i2 < 10; ++i2) {
      int i = i1 * 10 + i2;
      A[i] = A[i] + 1;
   }
}

void step2(double A[100])
{
   for (int i1 = 0; i1 < 10; ++i1)
   #pragma omp reverse
   for (int i2 = 0; i2 < 5; ++i2) {
      int i = i1 * 10 + i2 * 2;
      A[i] = A[i] + 1;
      ++i;
      A[i] = A[i] + 1;
   }
}

void step3(double A[100])
{
   for (int i1 = 0; i1 < 10; ++i1)
   for (int i2 = 4; i2 >= 0; --i2) {
      int i = i1 * 10 + i2 * 2;
      A[i] = A[i] + 1;
      ++i;
      A[i] = A[i] + 1;
   }
}
