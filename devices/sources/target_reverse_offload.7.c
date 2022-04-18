/*
* @@name:	target_reverse_offload.7
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
* @@version:	omp_5.2
*/

#include <stdio.h>
#include <stdlib.h>

#define N 100

#pragma omp requires reverse_offload

void error_handler(int wrong_value, int index)
{
   printf(" Error in offload: A[%d]=%d\n", index,wrong_value);
   printf("        Expecting: A[i ]=i\n");
   exit(1);
// output:  Error in offload: A[99]=-1
//                 Expecting: A[i ]=i

}
#pragma omp declare target device_type(host) enter(error_handler)
 
int main()
{
   int A[N];
 
   for (int i=0; i<N; i++) A[i] = i;

   A[N-1]=-1;
 
   #pragma omp target map(A)
   {
      for (int i=0; i<N; i++)
      {
         if (A[i] != i) 
         {
            #pragma omp target device(ancestor: 1) map(always,to: A[i:1])
               error_handler(A[i], i);
         }
      }
   }
   return 0;
}
