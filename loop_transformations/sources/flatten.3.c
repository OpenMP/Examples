/*
* @@name:       flatten.3
* @@type:       C
* @@operation:  run
* @@expect:     success
* @@version:    omp_6.1
*/
#define Ni  128
#define Nj  128
void flatten(unsigned n, float A[][Nj]){
  #pragma omp flatten depth(2)
  for(  unsigned i = 0; i< n; i++){
    for(unsigned j = 0; j<=i; j++){
        A[i][j] = 0;
    }
  }
}

void flatten_equivalent_empty_iterations(unsigned n, float A[][Nj]){
  if (n > 0) // at least one inner iteration
    for(unsigned int ij = 0; ij<n*n; ij++){
      unsigned int i = ij / n;
      unsigned int j = ij % n;
      if (j <= i) {
        // user code
        A[i][j] = 0;
      } else {
        // empty iteration
      }
    }
}

int main(){
 float A[Ni][Nj];
 flatten(Ni,A);
 flatten_equivalent_empty_iterations(Ni,A);
 return 0;
}
