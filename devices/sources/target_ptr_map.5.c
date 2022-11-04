/*
* @@name:	target_ptr_map.5
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.2
*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef struct {
   int *ptr;
   int buf_size;
} T;

#pragma omp declare mapper(deep_copy: T s) map(s, s.ptr[:s.buf_size])

void do_work(int *ptr, const int size);

int main() 
{
   const int n = 1000;
   const int buf_size = sizeof(int) * n;
   T s = { 0, buf_size };   
   const int dev = omp_get_default_device();
   s.ptr = (int *)malloc(buf_size);
   const int accessible =
      omp_target_is_accessible(s.ptr, s.buf_size, dev);

   #pragma omp metadirective \
      when(user={condition(accessible)}: target) \
      otherwise(target map(mapper(deep_copy),tofrom:s) )
   {
      do_work(s.ptr, n);
   } 

   free(s.ptr);
   return 0;
}
