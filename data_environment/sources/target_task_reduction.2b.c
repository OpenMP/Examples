/*
* @@name:	target_task_reduction.2b
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.2
*/
#include <stdio.h>
extern void device_compute(int *);
#pragma omp declare target enter(device_compute)
extern void host_compute(int *);
int main()
{
   int sum = 0;

   #pragma omp parallel masked reduction(task, +:sum)
   {
       #pragma omp target in_reduction(+:sum) nowait
       device_compute(&sum);

       host_compute(&sum);
   }
   printf(  "sum = %d\n", sum);
   //OUTPUT: sum = 2
   return 0;
}

void device_compute(int *sum){ *sum = 1; }
void   host_compute(int *sum){ *sum = 1; }
