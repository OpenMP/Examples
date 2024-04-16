/*
* @@name:       usm_scalar_ptr_ref_asc.1
* @@type:       C++ 
* @@operation:  run
* @@expect:     success
* @@version:    omp_5.2
*/
#include <stdio.h>

#pragma omp requires unified_shared_memory

int main(){
  int x = 0;     // scalar
  int *ptr = &x; // pointer to a scalar 
  int &ref = x;  // reference to a scalar

  bool pass = true;

  // Case 1: x is firstprivate
  #pragma omp target 
  {
     x++;
  }
  if( x != 0 ) pass = false;

  x = 0;
  // Case 2: ptr is firstprivate
  //         (uses address assigned in host data environment)
  #pragma omp target
  {
     (*ptr)++;
  }
  if( x != 1 ) pass = false;

  x = 0;
  // Case 3: ref and its object are mapped
  #pragma omp target
  {
    ref++;
  }
  if( x != 1 ) pass = false;

  // Verification
  if( pass ) { printf("PASSED\n"); return 0; }
  else       { printf("FAILED\n"); return 1; }

}
