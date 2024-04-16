/* 
* @@name:	loop.2
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.0
*/
#include  <stdio.h>
#define N 1024

int x[N][N];
int y[N], z[N]; 

void foo() {
// i-loop distributed across encountering league of teams
#pragma omp loop bind(teams)
    for (int i = 0; i < N; i++) {
    // this loop has an implicit bind(thread)
    #pragma omp loop
      for (int j = 0; j < N; j++) {
        x[i][j] += y[i]*z[i];
      }
    }
}

int main(){
 int error = 0;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
       x[i][j] = 0;
    }
  }

  for (int i = 0; i < N; i++) {
    y[i] = i;
    z[i] = i+1;
  }

#pragma omp teams num_teams(4) 
  {
      foo();
  }

//check values
   for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
       if( x[i][j] != i * (i+1))
         error++;
    }
  }
  if(error) {
    printf("FAILED\n");
    return 1;
  }    
  printf("PASSED\n");   
  return 0;
} 
