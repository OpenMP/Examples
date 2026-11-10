/*
* @@name:	cond_comp.1
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	pre_omp_3.0
*/
#include <stdio.h>
#ifdef _OPENMP
#include <omp.h>
#endif

int main()
{
   int iam = 0, index = 100;

#ifdef _OPENMP
   iam = omp_get_thread_num() + index;
   printf("Compiled by an OpenMP-compliant implementation.\n");
#endif
   printf("iam = %d\n", iam);

   return 0;
}
