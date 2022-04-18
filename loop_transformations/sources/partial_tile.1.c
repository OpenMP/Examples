/*
* @@name:       partial_tile.1
* @@type:       C
* @@compilable: yes
* @@linkable:   no
* @@expect:     success
* @@version:    omp_5.1
*/
int min(int a, int b){ return (a < b)? a : b; }

void func1(double A[100][100])
{
   #pragma omp tile sizes(4,16)
   for (int i = 0; i < 100; ++i)
       for (int j = 0; j < 100; ++j)
          A[i][j] = A[i][j] + 1;
}

void func2(double A[100][100])
{
   for (int i1 = 0; i1 < 100; i1+=4)
       for (int j1 = 0; j1 < 100; j1+=16)
           for (int i2 = i1; i2 < i1+4; ++i2)
               for (int j2 = j1; j2 < min(j1+16,100); ++j2)
                  A[i2][j2] = A[i2][j2] + 1;
}

void func3(double A[100][100])
{
   // complete tiles
   for (int i1 = 0; i1 < 100; i1+=4)
       for (int j1 = 0; j1 < 96; j1+=16)
           for (int i2 = i1; i2 < i1+4; ++i2)
               for (int j2 = j1; j2 < j1+16; ++j2)
                  A[i2][j2] = A[i2][j2] + 1;
   // partial tiles / remainder
   for (int i1 = 0; i1 < 100; i1+=4)
       for (int i2 = i1; i2 < i1+4; ++i2)
           for (int j = 96; j < 100; j+=1)
                  A[i2][j] = A[i2][j] + 1;
}

void func4(double A[100][100])
{
   for (int i1 = 0; i1 < 100; i1+=4) {
       // complete tiles
       for (int j1 = 0; j1 < 96; j1+=16)
           for (int i2 = i1; i2 < i1+4; ++i2)
               for (int j2 = j1; j2 < j1+16; ++j2)
                  A[i2][j2] = A[i2][j2] + 1;
       // partial tiles     
       for (int i2 = i1; i2 < i1+4; ++i2)
           for (int j = 96; j < 100; j+=1)
                  A[i2][j] = A[i2][j] + 1;
   }
}

void func5(double A[100][100])
{
   for (int i1 = 0; i1 < 100; i1+=4)
       for (int j1 = 0; j1 < 100; j1+=16)
           for (int i2 = i1; i2 < i1+4; ++i2)
               for (int j2 = j1; j2 <j1+16; ++j2)
                  if (j2 < 100)
                      A[i2][j2] = A[i2][j2] + 1;
}

void func6(double A[100][100])
{
   // complete tiles
   for (int i1 = 0; i1 < 100; i1+=4)
       for (int j1 = 0; j1 < 96; j1+=16)
           for (int i2 = i1; i2 < i1+4; ++i2)
               for (int j2 = j1; j2 < j1+16; ++j2)
                  A[i2][j2] = A[i2][j2] + 1;
   // partial tiles / remainder (not tiled)
   for (int i = 0; i < 100; ++i)
       for (int j = 96; j < 100; ++j)
                  A[i][j] = A[i][j] + 1;
}
