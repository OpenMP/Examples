/*
* @@name:       metadirective.4c
* @@type:       C
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version:    omp_5.1
*/
#define N 100
#include <stdbool.h>
#include   <stdio.h>
#include     <omp.h>

void foo(int *a, int n, bool use_gpu)
{
   int b=0;   //  use b to detect if run on gpu

   #pragma omp metadirective \
               when( user={condition(use_gpu)}:           \
                     target teams distribute parallel for \
                     private(b) map(from:a[0:n]) )        \
               default( parallel for )
   for (int i=0; i<n; i++) {a[i]=i; if(i==n-1) b=1;}

   if(b==0) printf("PASSED 1 of 3\n"); 
}

void bar (int *a, int n, bool run_parallel, bool unbalanced)
{
   int b=0;
   #pragma omp metadirective when(user={condition(run_parallel)}: parallel)
   {
      if(omp_in_parallel() == 1 && omp_get_thread_num() == 0) 
      {printf("PASSED 2 of 3\n");}

      #pragma omp metadirective \
          when( construct={parallel}, \
                user={condition(unbalanced)}: for schedule(guided) private(b)) \
          when( construct={parallel}        : for schedule(static))
      for (int i=0; i<n; i++) {a[i]=i; if(i==n-1) b=1;}
   }
   if(b==0) printf("PASSED 3 of 3\n"); //if guided b=0, because b is private
}

void foo(int *a, int n, bool use_gpu);
void bar(int *a, int n, bool run_parallel, bool unbalanced);

int main(){

   int p[N];
                       // App normally sets these, dependent on input parameters
   bool use_gpu=true, run_parallel=true, unbalanced=true;

                       // Testing: set Env Var MK_FAIL to anything to fail tests
   if(getenv("MK_FAIL")!=NULL) {
      use_gpu=false; run_parallel=false; unbalanced=false;
   }

   foo(p, N, use_gpu);
   bar(p, N, run_parallel,unbalanced);

}
