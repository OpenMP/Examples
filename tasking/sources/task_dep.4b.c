/*
* @@name:	task_dep.4b
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.1
*/
#include <stdio.h>

extern int f(int i);

void task_dep(int N)
{
  int i, v, R;

  #pragma omp parallel private(i,v) shared(R)
  #pragma omp single
  {
    // CASE 1: tasks with inout dependence type.
    //         tasks are serialized here.
    R = 0;
    for ( i = 0; i < N; i++ ) {
      #pragma omp task depend(inout: R)    // T1
      {
        v = f(i);
        R += v;
      }
    }

    #pragma omp task depend(in: R)         // T2
      printf("result is %d\n", R);
    #pragma omp taskwait      // to avoid race with CASE 2

    // CASE 2: tasks with inoutset dependence type.
    //         tasks are executed concurrently.
    R = 0;
    for ( i = 0; i < N; i++ ) {
      #pragma omp task depend(inoutset: R) // T3
      {
        v = f(i);
        #pragma omp atomic
        R += v;
      }
    }

    #pragma omp task depend(in: R)         // T4
      printf("result is %d\n", R);
  }
}
