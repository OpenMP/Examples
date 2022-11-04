/*
* @@name:	task_dep.13
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.1
*/
#include <stdio.h>

int main(){
   int a=1, d=1;

   #pragma omp parallel masked num_threads(5)
   {   
      #pragma omp task                               // Task 1
      { printf("T1\n"); }

      #pragma omp task depend(out: a)                // Task 2
      { a++;
        printf("T2 a=%i\n", a); }

      #pragma omp task depend(out: d)                // Task 3
      { d++;
        printf("T3 d=%i\n", d); }

      #pragma omp task depend(inout: omp_all_memory) // Task 4
      { a++; d++; 
        printf("T4 a=%i d=%i\n",   a,d);}

      #pragma omp task                               // Task 5
      { printf("T5\n"); }

      #pragma omp task depend(in: a,d)               // Task 6
      { a++; d++;
        printf("T6 a=%i d=%i\n", a,d); }
   }   
}

/* OUTPUT: ordered {T2,T3 any order}, {T4}, {T6}
    T2 a=2
    T3 d=2
    T4 a=3 d=3
    T6 a=4 d=4

  OUTPUT: unordered (can appear interspersed in ordered output)
    T1
    T5
*/
