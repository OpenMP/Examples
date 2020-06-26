/* 
* @@name: loop.2c
* @@type: C
* @@compilable: yes
* @@linkable: yes
* @@expect: success
* @@version: omp_5.0
*/
#include  <stdio.h>
#define N 100
int main() 
{
  float x[N], y[N];
  float a = 2.0;
  for(int i=0;i<N;i++){ x[i]=i; y[i]=0;}   // initialize
  
  #pragma omp parallel
  {
    #pragma omp loop
    for(int i = 0; i < N; ++i) y[i] = a*x[i] + y[i];
  }
  if(y[N-1] != (N-1)*2.0) printf("Error: 2*(N-1) != y[N-1]=%f",y[N-1]);
}   
