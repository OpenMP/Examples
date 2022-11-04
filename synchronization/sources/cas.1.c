/*
* @@name:	cas.1
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.1
*/
#include <stdio.h>
#define N 10

void init(int *);

int main(){
  int val_min=2*N, val_max=-2*N; 
  int val[N];

  init(val);

  #pragma omp parallel for num_threads(2)
  for (int i=1; i<N-1; i++) {

    // compare and update val_min using one atomic form
    #pragma omp atomic compare
    if (val[i] < val_min) { val_min = val[i]; }

    // compare and update val_max using another atomic form
    #pragma omp atomic compare
    val_max = val[i] > val_max ? val[i] : val_max;
  }

  if(val_max != 2*N || val_min != -2*N){ printf("FAILED\n");}
  else                                 { printf("PASSED\n");}
  // OUT: PASSED
}

void init(int *val){
  for (int i=0; i<N; i++) val[i]=i; 
  val[N/2  ] =  2*N;
  val[N/2+1] = -2*N;
}
