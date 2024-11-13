/*
* @@name:	async_target.5
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.0
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#pragma omp begin declare target
void update(int* num) {
 
    *num = (*num) * 3;
}
#pragma omp end declare target

int main(int argc, char*argv[]){
  int x = 2 ;
  int is_deferred = time(NULL) % 2;

  #pragma omp target nowait(is_deferred) map(tofrom: x)
  {
    update(&x); 
  }

  // Perform other tasks in parallel while the 
  // target region is executing  
 
  if(is_deferred){
    #pragma omp taskwait
  }

  if( x == 6){
    printf("Passed\n");
    return 0;
  } else {
    printf("Failed\n");
    return 1;
  }
}
