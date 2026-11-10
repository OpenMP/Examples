/*
* @@name:       flatten.4
* @@type:       C
* @@operation:  compile
* @@expect:     success
* @@version:    omp_6.1
*/
void flatten(float A[][128]){
  #pragma omp for schedule(static)
  #pragma omp flatten depth(2)
  for(  int i = 0; i<128; i++){
    for(int j = 0; j<=i;  j++){
      A[i][j] = 0;
    }
  }
}
