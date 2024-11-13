/*
* @@name:	apply_syntax_equivalent.3
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
void apply_complexarg_equivalent1(double A[100*100])
{
   #pragma omp tile sizes(4,5)     \
      apply(grid: reverse,nothing) \
      apply(intratile: nothing,unroll)
   for (int i = 0; i < 100; ++i)
      for (int j = 0; j < 100; ++j)
         A[i*100+j] += 1;
}

void apply_complexarg_equivalent2(double A[100*100])
{
   #pragma omp reverse
   for (int i1 = 0; i1 < 100; i1+=4)          // grid loop 1
      for (int j1 = 0; j1 < 100; j1+=5)       // grid loop 2
         for (int i = i1; i < i1+4; i+=1)     // tile loop 1
            #pragma omp unroll
            for (int j = j1; j < j1+5; j+=1)  // tile loop 2
               A[i*100+j] += 1;
}
