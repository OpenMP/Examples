/*
* @@name:	metadirective.3a
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_5.2
*/
#include <stdio.h>
#include <math.h>
#define  N 1000

#pragma omp begin declare target
// This procedure is expected to be called from within a target teams or
// outside a target region from a parallel region and will distribute
// iterations across the league or team, respectively. 
void exp_pi_diff(double *d, double my_pi)
{
  #pragma omp metadirective \
              when( construct={target}: loop bind(teams) ) \
              otherwise( loop bind(parallel) )
  for(int i = 0; i < N; i++)
    d[i] = exp( (M_PI-my_pi)*i );
}

#pragma omp end declare target

int main()
{
  // Calculates sequence of exponentials: (M_PI-my_pi) * index
  // M_PI is from math.h, and my_pi is user provided.

  double d[N];
  double my_pi=3.14159265358979e0;

  // Case 1: exp_pi_diff called from a target teams region, so that array is
  // computed across league of teams. Note, however, that a target teams region
  // that falls back to host execution may not work if the callee depends on
  // the target trait.

  #pragma omp target teams map(from: d[0:N])
  exp_pi_diff(d,my_pi);
  // value should be near 1
  printf("d[N-1] = %20.14f\n",d[N-1]); // ...= 1.00000000000311

  // Case 2: exp_pi_diff called from a parallel region, so that array is
  // computed across team of threads

  #pragma omp parallel
  exp_pi_diff(d,my_pi);
  // value should be near 1
  printf("d[N-1] = %20.14f\n",d[N-1]); // ...= 1.00000000000311

  return 0;
}
