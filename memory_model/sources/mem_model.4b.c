/*
* @@name:	mem_model.4b
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_3.1
*/
#include <omp.h>

void flush_correct()
{
  int a, b;
  a = b = 0;
  #pragma omp parallel num_threads(2)
  {
    int myid = omp_get_thread_num();
    int tmp;

    if ( myid == 0 ) {          // thread 0
      #pragma omp atomic write
        b = 1;
      #pragma omp flush(a,b)    // flushes are ordered
      #pragma omp atomic read
        tmp = a;
    }
    else {                      // thread 1
      #pragma omp atomic write
        a = 1;
      #pragma omp flush(a,b)    // flushes are ordered
      #pragma omp atomic read
        tmp = b;
    }
    if ( tmp == 0 ) {           // access by single thread
      /* protected section */
    }
  }
}
