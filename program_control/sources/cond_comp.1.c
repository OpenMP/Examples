/*
* @@name:	cond_comp.1
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	pre_omp_3.0
*/
#include <stdio.h>

int main()
{

# ifdef _OPENMP
    printf("Compiled by an OpenMP-compliant implementation.\n");
# endif

    return 0;
}
