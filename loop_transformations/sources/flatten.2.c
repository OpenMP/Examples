/*
* @@name:       flatten.2
* @@type:       C
* @@operation:  run
* @@expect:     success
* @@version:    omp_6.1
*/
#include <stdint.h>

#define Ni 3
#define Nj 5
void flatten_unroll(int A[][Nj]){
  #pragma omp unroll partial(4)
  #pragma omp flatten depth(2)
    for(  int i = 0; i<Ni; i++){
      for(int j = 0; j<Nj; j++){
        A[i][j] = 0;
      }
    }
}


void flatten_unroll_equivalent(int A[][Nj]){
  int *flat_A = (int *)A;

  for(int i = 0; i<(Ni*Nj)/4; ++i){
    flat_A[i*4 + 0] = 0;
    flat_A[i*4 + 1] = 0;
    flat_A[i*4 + 2] = 0;
    flat_A[i*4 + 3] = 0;
  }

  for(int i = ((Ni*Nj)/4)*4; i<Ni*Nj; ++i){
    flat_A[i] = 0;
  }
}

int main(){
  int A[Ni][Nj];
  flatten_unroll(A);
  flatten_unroll_equivalent(A);
}
