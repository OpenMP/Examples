/*
* @@name: task_depend.6.c
* @@type: C
* @@compilable: yes
* @@linkable: yes
* @@expect: success
* @@version: omp_5.0
*/

#include<stdio.h>

void foo()
{
    int x = 0, y = 2;

    #pragma omp task depend(inout: x) shared(x)
    x++;                                        // 1st child task

    #pragma omp task shared(y)
    y--;                                        // 2nd child task

    #pragma omp taskwait depend(in: x)          // 1st taskwait

    printf("x=%d\n",x);

    // Second task may not be finished.
    // Accessing y here will create a race condition.

    #pragma omp taskwait                        // 2nd taskwait

    printf("y=%d\n",y);
}

int main()
{
    #pragma omp parallel
    #pragma omp single
    foo();

    return 0;
}
