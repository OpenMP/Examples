/*
* @@name:	metadirective.5
* @@type:	C++
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.0
*/
#include <stdio.h>

// revised Fibonacci from tasking.4.c example

template <bool tasking>
int fib(int n) {
  int i, j;
  if (n<2) {
    return n;
  } else if ( tasking && n<8 ) { // serial/taskless cutoff for n<8
    return fib<false>(n);
  } else {
    #pragma omp metadirective \
                when(user={condition(tasking)}: task shared(i))
    {
      i=fib<tasking>(n-1);
    }
    #pragma omp metadirective \
                when(user={condition(tasking)}: task shared(j))
    {
      j=fib<tasking>(n-2);
    }
    #pragma omp metadirective \
                when(user={condition(tasking)}: taskwait)
    return i+j;
  }
}

int main(int argc, char** argv) {
  int n = 15;
  #pragma omp parallel
  #pragma omp single
  {
    printf("fib(%i) = %i\n", n, fib<true>(n));
  }
  return 0;
}
// OUTPUT:
// fib(15) = 610
