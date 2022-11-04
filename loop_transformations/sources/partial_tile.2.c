/*
* @@name:	partial_tile.2
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.1
*/
int min(int a, int b){ return (a < b)? a : b; }

void func7(double A[100][100])
{
   #pragma omp parallel for schedule(static)
   #pragma omp tile sizes(4,16)
   for (int i = 0; i < 100; ++i)
       for (int j = 0; j < 100; ++j)
           A[i][j] = A[i][j] + 1;
}

void func8(double A[100][100])
{
   #pragma omp parallel for schedule(static)
   for (int i1 = 0; i1 < 100; i1+=4)
       for (int j1 = 0; j1 < 100; j1+=16)
           for (int i2 = i1; i2 < i1+4; ++i2)
               for (int j2 = j1; j2 < min(j1+16,100); ++j2)
                  A[i2][j2] = A[i2][j2] + 1;
}

void func9(double A[100][100])
{
   #pragma omp parallel
   {
       #pragma omp for schedule(static) nowait
       for (int i1 = 0; i1 < 100; i1+=4)
           for (int j1 = 0; j1 < 96; j1+=16)
               for (int i2 = i1; i2 < i1+4; ++i2)
                   for (int j2 = j1; j2 < j1+16; ++j2)
                      A[i2][j2] = A[i2][j2] + 1;
       #pragma omp for schedule(static)
       for (int i1 = 0; i1 < 100; i1+=4) 
           for (int i2 = i1; i2 < i1+4; ++i2) 
               for (int j = 96; j < 100; j+=1)
                   A[i2][j] = A[i2][j] + 1;
    }
}
