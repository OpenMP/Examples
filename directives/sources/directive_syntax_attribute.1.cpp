/*
* @@name:       directive_syntax_attribute.1
* @@type:       C++
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version:    omp_5.0
*/
#include <stdio.h>
#include <omp.h>
#define NT 4
#define thrd_no omp_get_thread_num

#pragma omp declare simd linear(i) simdlen(4)
#pragma omp declare simd linear(i) simdlen(8)
double P(int i){ return (double)i * (double)i; }

[[omp::directive(declare simd linear(i) simdlen(4))]]
[[omp::directive(declare simd linear(i) simdlen(8))]]
double Q(int i){ return (double)i * (double)i; }

int main() {

    #pragma omp parallel for num_threads(NT)                 // PRAG 1
    for(int i=0; i<NT; i++) printf("thrd no %d\n",thrd_no());

    #pragma omp parallel num_threads(NT)                     // PRAG 2
    #pragma omp for                                          // PRAG 3
    for(int i=0; i<NT; i++) printf("thrd no %d\n",thrd_no());

                                                             // ATTR 1
    [[omp::directive( parallel for num_threads(NT))]]
    for(int i=0; i<NT; i++) printf("thrd no %d\n",thrd_no());

                                                             // ATTR 2
    [[using omp : directive( parallel for num_threads(NT))]]
    for(int i=0; i<NT; i++) printf("thrd no %d\n",thrd_no());

 // INVALID-- attribute and non-attribute on same statement
 // [[ omp :: directive( parallel num_threads(NT) ) ]]          ATTR 3
 // #pragma omp for                                             PRAG 4 
 // for(int i=0; i<NT; i++) printf("thrd no %d\n",thrd_no());


 // INVALID-- directive order not guaranteed 
 // [[ omp :: directive( parallel num_threads(NT) ) ]]          ATTR 4
 // [[ omp :: directive( for                      ) ]]          ATTR 5
 // for(int i=0; i<NT; i++) printf("thrd no %d\n",thrd_no());

                                                             // ATTR 6
    [[omp::sequence(directive(parallel num_threads(NT)),directive(for))]]
    for(int i=0; i<NT; i++) printf("thrd no %d\n",thrd_no());

    double tmp=0.0f;
    #pragma omp simd reduction(+:tmp) simdlen(4)
    for(int i=0;i<100;i++) tmp += P(i);                 // PRAG DECL 1
    #pragma omp simd reduction(+:tmp) simdlen(8)
    for(int i=0;i<100;i++) tmp += P(i);                 // PRAG DECL 2
    printf("%f\n",tmp);

    tmp=0.0f;
    #pragma omp simd reduction(+:tmp) simdlen(4)
    for(int i=0;i<100;i++) tmp += Q(i);                 // ATTR DECL 3
    #pragma omp simd reduction(+:tmp) simdlen(8)
    for(int i=0;i<100;i++) tmp += Q(i);                 // ATTR DECL 4
    printf("%f\n",tmp);

    tmp=0.0f;
    [[ omp :: directive(simd reduction(+:tmp) simdlen(4))]]
    for(int i=0;i<100;i++) tmp += Q(i);                 // ATTR DECL 5
    [[ omp :: directive(simd reduction(+:tmp) simdlen(8))]]
    for(int i=0;i<100;i++) tmp += Q(i);                 // ATTR DECL 6
    printf("%f\n",tmp);
}
//    repeated 5 times, any order:
//    OUTPUT: thrd no  0
//    OUTPUT: thrd no  1
//    OUTPUT: thrd no  2
//    OUTPUT: thrd no  3

//    repeated 3 time:
//    OUTPUT: 656700.000000

