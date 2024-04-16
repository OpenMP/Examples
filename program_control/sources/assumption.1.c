/*
* @@name:       assumption.1
* @@type:       C
* @@operation:  compile
* @@expect:     success
* @@version:    omp_5.1
*/

#include <stdio.h>
#include <stdlib.h>

#pragma omp declare target
int N;
#pragma omp end declare target

// Case 1: Delimited scope
#pragma omp begin assumes no_parallelism
extern void fun(int *A);
#pragma omp end assumes

int main() {
    int *A, *B;
    N = (rand() % 5 + 1) * 16;
    A = (int *) malloc(sizeof(int) * N);
    B = (int *) malloc(sizeof(int) * N);

    for(int i = 0; i < N; i++){
        A[i] = 0;
        B[i] = i;
    }

    #pragma omp target teams distribute parallel for map(tofrom: A[0:N]) 
    for(int i = 0; i < N; i++){
      fun(A);
    }

// Case 2: Block associated

    #pragma omp assume holds (N % 8 == 0 && N > 0)
    #pragma omp simd
    for (int i = 0; i < N; ++i){
      A[i] += B[i];
    }

    return 0;
}

