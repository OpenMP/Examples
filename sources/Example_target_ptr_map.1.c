/*
* @@name:       target_ptr_map.1
* @@type:       C
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version:	omp_5.0
*/
#include <stdio.h>
#include <stdlib.h>
#define N 100

int main()
{
  int *ptr1;
  int *ptr2;
  int *ptr3;
  int aray[N];

  ptr1 = (int *)malloc(sizeof(int)*N);
  ptr2 = (int *)malloc(sizeof(int)*N);

  #pragma omp target map(ptr1, ptr1[:N]) map(ptr2[:N] )
  {
     for (int i=0; i<N; i++)
     {
         ptr1[i] = i;
         ptr2[i] = i;
         aray[i] = i;
     }

   //*(++ptr1) = 9;  //NOT ALLOWED since ptr1 is an attached pointer
     *(++ptr2) = 9;  //    allowed since ptr2 is firstprivate

     ptr3=(int *)malloc(sizeof(int)*N); // ptr3 is firstprivate
                                        // ptr3 value not returned
     for (int i=0; i<N; i++) ptr3[i] = 5;

     for (int i=0; i<N; i++) ptr1[i] += ptr3[i];

     free(ptr3);     // explicitly free allocated storage on device
   }

   printf(" %d %d\n",ptr1[1],ptr2[1]);
   //        6  9

   free(ptr1);
   free(ptr2);
   return 0;
}
