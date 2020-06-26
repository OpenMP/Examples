/*
* @@name:   taskloop.2c
* @@type:   C
* @@compilable: yes
* @@linkable:   no
* @@expect: success
* @@version: omp_4.5
*/
#include <stdio.h>

#define T 16
#define N 1024

void parallel_work() {
    int x1 = 0, x2 = 0;

    #pragma omp parallel shared(x1,x2) num_threads(T)
    {
        #pragma omp taskloop
        for (int i = 0; i < N; ++i) {
            #pragma omp atomic
            x1++;          // executed T*N times
        }

        #pragma omp single
        #pragma omp taskloop
        for (int i = 0; i < N; ++i) {
            #pragma omp atomic
            x2++;          // executed N times
        }
    }

    printf("x1 = %d, x2 = %d\n", x1, x2);
}
