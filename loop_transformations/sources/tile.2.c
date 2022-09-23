/*
* @@name:       tile.2
* @@type:       C
* @@compilable: yes
* @@linkable:   no
* @@expect:     success
* @@version:    omp_5.1
*/
void func1(int A[100][128])
{
   #pragma omp tile sizes(4, 4)
   #pragma omp tile sizes(5,16)
   for (int i = 0; i < 100; ++i)
       for (int j = 0; j < 128; ++j)
           A[i][j] = i*1000 + j;
}

void func2(int A[100][128])
{
   #pragma omp tile sizes(4,4)
   for (int i1 = 0; i1 < 100; i1+=5)
       for (int j1 = 0; j1 < 128; j1+=16)
           for (int i2 = i1; i2 < i1+5; ++i2)
               for (int j2 = j1; j2 < j1+16; ++j2)
                  A[i2][j2] = i2*1000 + j2;
}

void func3(int A[100][128])
{
   for (int i11 = 0; i11 < 100; i11+= 5*4)
   for (int j11 = 0; j11 < 128; j11+=16*4)

       for (int i12 = i11; i12 < i11+( 5*4); i12+= 5)
       for (int j12 = j11; j12 < j11+(16*4); j12+=16)

           for (int i2 = i12; i2 < i12+ 5; ++i2)
           for (int j2 = j12; j2 < j12+16; ++j2)
              A[i2][j2] = i2*1000 + j2;
}
