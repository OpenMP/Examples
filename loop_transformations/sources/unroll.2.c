/*
* @@name:	unroll.2c
* @@type:	C
* @@compilable:	no
* @@linkable:	no
* @@expect:	success
* @@version:    omp_5.1
*/
void illegal_2a(double A[])
{
    #pragma omp for
    #pragma omp unroll full  // ERROR: Full unrolling does not leave a loop.
    for (int i = 0; i < 12; ++i)
        A[i] = 0;
}

void illegal_2b(double A[])
{
    // Loop might be fully unrolled* (or a partially unrolled loop replacement)
    // *Hence, no canonical for-loop, resulting in non-compliant code.
    // Implementations may suggest to adding a "partial" clause.

    #pragma omp for          //        Requires a canonical loop
    #pragma omp unroll       // ERROR: may result in non-compliant code
    for (int i = 0; i < 12; ++i)
        A[i] = 0;
}

void illegal_2c(int n, double A[])
{
    #pragma omp unroll full  // Full unroll requires constant iteration count.
    for (int i = 0; i < n; ++i)
        A[i] = 0;
}
