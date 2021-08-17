/*
* @@name:       tile.1c
* @@type:       C
* @@compilable: yes
* @@linkable:   no
* @@expect:     success
* @@version:    omp_5.1
*/
void func1(int A[100][128])
{
   #pragma omp parallel for 
   #pragma omp tile sizes(5,16)
   for (int i = 0; i < 100; ++i)
       for (int j = 0; j < 128; ++j)
           A[i][j] = i*1000 + j;
}

void func2(int A[100][128])
{
   #pragma omp parallel for
   for (int i1 = 0; i1 < 100; i1+=5)
       for (int j1 = 0; j1 < 128; j1+=16)
           for (int i2 = i1; i2 < i1+5; ++i2)
               for (int j2 = j1; j2 < j1+16; ++j2)
                  A[i2][j2] = i2*1000 + j2;
}
