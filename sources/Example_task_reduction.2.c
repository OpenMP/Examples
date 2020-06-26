/*
* @@name:       task_reduction.2.c
* @@type:       C
* @@compilable: yes, omp_5.0
* @@linkable:   yes
* @@expect:     success
* @@version:	omp_5.0
*/
#include <stdio.h>
int main(void){
   int N=100, M=10;
   int i, x;

// USE CASE 1  explicit-task reduction + parallel reduction clause
   x=0;
   #pragma omp parallel num_threads(M) reduction(task,+:x) 
   { 

     x++;                // implicit task reduction statement

     #pragma omp single
     for(i=0;i<N;i++)
       #pragma omp task in_reduction(+:x)
       x++;
   
   }
   printf("x=%d  =M+N\n",x);  // x= 110  =M+N


// USE CASE 2  task reduction +  worksharing reduction clause
   x=0;
   #pragma omp parallel for num_threads(M) reduction(task,+:x)
   for(i=0; i< N; i++){   

      x++;

      if( i%2 == 0){
       #pragma omp task in_reduction(+:x)
       x--;
      }
   }
   printf("x=%d  =N-N/2\n",x);  // x= 50  =N-N/2

   return 0;
}
