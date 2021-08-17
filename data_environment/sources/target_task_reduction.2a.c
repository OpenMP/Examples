/*
* @@name: target_task_reduction.2.c
* @@type: C
* @@compilable: yes
* @@linkable: yes
* @@expect: success
* @@version: omp_5.0
*/
#include <stdio.h>
#pragma omp declare target to(device_compute)
extern void device_compute(int *);
extern void host_compute(int *);
int main()
{
   int sum = 0;

   #pragma omp parallel sections reduction(task, +:sum)
   {
      #pragma omp section
          {
             #pragma omp target in_reduction(+:sum)
             device_compute(&sum);
          }
      #pragma omp section
          {
             host_compute(&sum);
          }
   }
   printf(  "sum = %d\n", sum);
   //OUTPUT: sum = 2
   return 0;
}

void device_compute(int *sum){ *sum = 1; }
void   host_compute(int *sum){ *sum = 1; }
