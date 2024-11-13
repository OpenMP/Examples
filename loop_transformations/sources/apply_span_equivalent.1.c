/*
* @@name:	apply_span_equivalent.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
void step1(double A[128][128])
{
   #pragma omp for collapse(2)
   #pragma omp interchange
   for (int i1 = 0; i1 < 8; ++i1)
   #pragma omp reverse
   for (int j1 = 0; j1 < 8; ++j1)

      for (int i2 = 0; i2 < 16; ++i2)
      for (int j2 = 0; j2 < 16; ++j2) {
         int i = i1 * 16 + i2;
         int j = j1 * 16 + j2;
         A[i][j] = A[i][j] + 1;
      }
}

void step2(double A[128][128])
{
   #pragma omp for collapse(2)
   #pragma omp interchange
   for (int i1 =  0; i1 < 8; ++i1)
   for (int j1 = 7; j1 >= 0; --j1)

      for (int i2 = 0; i2 < 16; ++i2)
      for (int j2 = 0; j2 < 16; ++j2) {
         int i = i1 * 16 + i2;
         int j = j1 * 16 + j2;
         A[i][j] = A[i][j] + 1;
      }
}

void step3(double A[128][128])
{
   #pragma omp for collapse(2)
   for (int j1 = 7; j1 >= 0; --j1)
   for (int i1 =  0; i1 < 8; ++i1)

      for (int i2 = 0; i2 < 16; ++i2)
      for (int j2 = 0; j2 < 16; ++j2) {
         int i = i1 * 16 + i2;
         int j = j1 * 16 + j2;
         A[i][j] = A[i][j] + 1;
      }

}
