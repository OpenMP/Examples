/*
* @@name:	tasking.4c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_3.0
*/
     int fib(int n) {
        int i, j;
        if (n<2)
          return n;
        else {
           #pragma omp task shared(i)
              i=fib(n-1);
           #pragma omp task shared(j)
              j=fib(n-2);
           #pragma omp taskwait
              return i+j;
        }
     }
