/*
* @@name:	cancellation.1c
* @@type:	C++
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.0
*/
#include <iostream>
#include <exception>
#include <cstddef>

#define N 10000

extern void causes_an_exception();
extern void phase_1();
extern void phase_2();

void example() {
    std::exception *ex = NULL;
#pragma omp parallel shared(ex)
    {
#pragma omp for
        for (int i = 0; i < N; i++) {
            // no 'if' that prevents compiler optimizations
            try {
                causes_an_exception();
            }
            catch (std::exception *e) {
                // still must remember exception for later handling
#pragma omp atomic write
                ex = e;
				// cancel worksharing construct
#pragma omp cancel for
            }
        }
   // if an exception has been raised, cancel parallel region
        if (ex) {
#pragma omp cancel parallel
        }
        phase_1();
#pragma omp barrier
        phase_2();
    }
    // continue here if an exception has been thrown in the worksharing loop
    if (ex) {
        // handle exception stored in ex
    }
}
