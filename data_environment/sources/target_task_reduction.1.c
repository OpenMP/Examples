/*
* @@name: target_task_reduction.1
* @@type: C
* @@compilable: yes
* @@linkable: yes
* @@expect: success
* @@version: omp_5.2
*/

#include <stdio.h>
#pragma omp declare target enter(device_compute)
void device_compute(int *);
void host_compute(int *);
int main()
{
   int sum = 0;

   #pragma omp parallel masked
   #pragma omp taskgroup task_reduction(+:sum)
   {
      #pragma omp target in_reduction(+:sum) nowait
          device_compute(&sum);

      #pragma omp task in_reduction(+:sum)
          host_compute(&sum);
   }
   printf(  "sum = %d\n", sum);
   //OUTPUT: sum = 2
   return 0;
}

void device_compute(int *sum){ *sum = 1; }
void   host_compute(int *sum){ *sum = 1; }
