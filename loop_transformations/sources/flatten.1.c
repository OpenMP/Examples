/*
* @@name:       flatten.1
* @@type:       C
* @@operation:  run
* @@expect:     success
* @@version:    omp_6.1
*/
#include <stdint.h>

#define Ni 3
#define Nj 4
#define Nk 3

void flatten(int A[][Nj][Nk]){
  #pragma omp flatten depth(2)
  for(    int i = 0; i<Ni; i++){
    for(  int j = 0; j<Nj; j++){
      for(int k = 0; k<Nk; k++){
        A[i][j][k] = 0;
      }
    }
  }
}

void flatten_equivalent(int A[][Nj][Nk]){
  int64_t tc_end = (int64_t)Ni * (int64_t)Nj;
  for(int64_t ij = 0; ij < tc_end; ij++){
    int i = ij/Nj;
    int j = ij%Nj;
    for(int k = 0; k<Nk; k++){
      A[i][j][k] = 0;
    }
  }
}

int main(){
 int A[Ni][Nj][Nk];
 flatten(A);
 flatten_equivalent(A);
 return 0;
}
