/*
* @@name:	apply_nested.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
void nested_apply(double A[100])
{
    #pragma omp tile sizes(10) \
            apply(intratile: unroll partial(2) apply(reverse))
    for (int i = 0; i < 100; ++i)
        A[i] = A[i] + 1;
}
