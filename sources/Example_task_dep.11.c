/*
* @@name:       task_dep.11c
* @@type:       C
* @@compilable: yes, omp_5.0
* @@linkable:   no
* @@expect:     success
*/

#include<stdio.h>

void set_an_element(int *p, int val) {
    *p = val;
}

void print_all_elements(int *v, int n) {
    int i;
    for (i = 0; i < n; ++i) {
        printf("%d, ", v[i]);
    }
    printf("\n");
}

void parallel_computation(int n) {
    int v[n];
    #pragma omp parallel
    #pragma omp single
    {
        int i;
        for (i = 0; i < n; ++i)
            #pragma omp task depend(out: v[i])
            set_an_element(&v[i], i);

        #pragma omp task depend(iterator(it = 0:n), in: v[it])
     // #pragma omp task depend(in: v[0:n]) Violates Array section restriction.
        print_all_elements(v, n);

    }
}

