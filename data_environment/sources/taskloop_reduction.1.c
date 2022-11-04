/*
* @@name:	taskloop_reduction.1
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.0
*/
#include <stdio.h>

int array_sum(int n, int *v) {
    int i;
    int res = 0;

    #pragma omp taskloop reduction(+: res)
    for(i = 0; i < n; ++i)
        res += v[i];

    return res;
}

int main(int argc, char *argv[]) {
    int n = 10;
    int v[10] = {1,2,3,4,5,6,7,8,9,10};

    #pragma omp parallel
    #pragma omp single
    {
        int res = array_sum(n, v);
        printf("The result is %d\n", res);
    }
    return 0;
}
