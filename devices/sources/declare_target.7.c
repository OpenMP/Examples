/*
* @@name: declare_target.7
* @@type: C
* @@operation: link
* @@expect: success
* @@version: omp_5.2
*/
#include <stdio.h>

void foo();
void foo_onhost();

#pragma omp declare target enter(foo) device_type(nohost)

#pragma omp declare variant(foo_onhost) match(device={kind(host)})
void foo(){
  // device specific computation
}

void foo_onhost(){
  printf("On host\n");
} 

int main(){
  #pragma omp target teams 
  {
    foo();  // calls foo() on target device 
            // or foo_onhost() in case of host fallback
  }
  return 0;

}
