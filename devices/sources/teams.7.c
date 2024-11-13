/*
* @@name:	teams.7
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.0
*/
#include<stdio.h>
#include<omp.h>

int x;
#pragma omp declare target local(x)

int main() {
  x = 128;
  #pragma omp target
  x = 256;

  #pragma omp target
  #pragma omp teams num_teams(x) // Undefined behavior due to value of "x"
  if (omp_get_team_num() == 0){
    printf("%d\n", omp_get_num_teams()); 
  }

  return 0;
}

