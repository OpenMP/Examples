/*
* @@name:       assumption.2
* @@type:       C
* @@operation:  compile
* @@expect:     success
* @@version:    omp_6.0
*/
#include <stdio.h>
#define N 5

void init(int *arr, int len);
int element_transform(int a);

int main() {
  int arr[N], arr_bang[N];

//Case 1: Use in sequential code
  #pragma omp assume no_openmp
  {   
    init(arr,N);  
  }   
    
//Case 2: Use inside openmp construct
  #pragma omp target teams loop map(to: arr)  map(from: arr_bang)
  for(int i = 0; i < N; i++) {
    #pragma omp assume no_parallelism 
    {   
       arr_bang[i] = element_transform(arr[i]);
    } 
  }
  printf("%d, %d\n", arr_bang[0], arr_bang[N-1]);
    
  return 0;
}