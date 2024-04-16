/*
* @@name:       declare_target_indirect_call.1
* @@type:       C   
* @@operation:  run 
* @@expect:     success
* @@version:    omp_5.2
*/
#include <stdio.h>
#include <stdlib.h>

typedef int(*funcptr)();

int fun1() {return 1;} 
int fun2() {return 2;} 
#pragma omp declare target enter(fun1, fun2) indirect 
                                          // indirect defaults to true
int main()
{
  int ret_val=0;
  const int choice = rand()%2 + 1;        // create runtime number 1 or 2
  
  funcptr fptr = (choice == 1) ? &fun1 : &fun2; //select fun1/fun2 for 1/2

  #pragma omp target map(from: ret_val)
    ret_val = fptr();                     // ret_val = 1/2 from fun1/fun2

  if (ret_val != choice) { printf("FAILED\n"); exit(1); }

  return 0;
}
