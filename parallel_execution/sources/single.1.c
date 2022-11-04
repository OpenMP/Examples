/*
* @@name:	single.1
* @@type:	C
* @@operation:	link
* @@expect:	success
* @@version:	pre_omp_3.0
*/
#include <stdio.h>

void work1() {}
void work2() {}

int main()
{
  #pragma omp parallel
  {
    #pragma omp single
      printf("Beginning work1.\n");

    work1();

    #pragma omp single
      printf("Finishing work1.\n");

    #pragma omp single nowait
      printf("Finished work1 and beginning work2.\n");

    work2();
  }
}
