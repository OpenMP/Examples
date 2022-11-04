/*
* @@name:	declare_target.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.1
*/
#pragma omp begin declare target
extern void fib(int N);
#pragma omp end declare target

#define THRESHOLD 1000000
void fib_wrapper(int n)
{
   #pragma omp target if(n > THRESHOLD)
   {
      fib(n);
   }
}
