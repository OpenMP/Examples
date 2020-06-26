/*
* @@name: task_depend.8.c
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
    x++;                                          // 1st child task

    #pragma omp task depend(in: x) depend(inout: y) shared(x, y)
    y -= x;                                       // 2st child task

    #pragma omp taskwait depend(in: x,y)

    printf("x=%d\n",x);
    printf("y=%d\n",y);

}

int main()
{
    #pragma omp parallel
    #pragma omp single
    foo();

    return 0;
}
