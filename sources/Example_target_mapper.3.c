/*
* @@name: target_mapper_map.3.c
* @@type: C
* @@compilable: yes
* @@linkable: no
* @@expect: success
* @@version: omp_5.0
*/

#include <stdlib.h>
#include  <stdio.h>

#define N 100

typedef struct myvec {
    size_t len;
    double *data;
} myvec_t;

#pragma omp declare mapper(myvec_t v) \
                    map(v, v.data[0:v.len])

typedef struct mypoints {
    struct myvec scratch;
    struct myvec *x;
    double hostonly_data[500000];
} mypoints_t;

#pragma omp declare mapper(mypoints_t v)  \
                    map(v.x, v.x[0] ) map(alloc:v.scratch)

void init_mypts_array(mypoints_t *P, int n);
void eval_mypts_array(mypoints_t *P, int n);

int main(){

   mypoints_t P;

   init_mypts_array(&P, N);

   #pragma omp target map(P)
   eval_mypts_array(&P, N);

}
