/*
* @@name:	metadirective.3b
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.0
*/
#include <stdio.h>
#define MATCH_CLAUSE match(construct={teams})
#include "metadirective.3b.h"
#undef MATCH_CLAUSE
#define MATCH_CLAUSE match(construct={parallel})
#include "metadirective.3b.h"
#undef MATCH_CLAUSE
#include "metadirective.3b.h"

#define  N 1000

int main()
{
  //Calculates sequence of exponentials: (M_PI-my_pi) * index
  //M_PI is from math.h, and my_pi is user provided.

  double d[N];
  double my_pi=3.14159265358979e0;

  // Case 1: exp_pi_diff called from a teams region, so that
  // array is computed across league of teams. This ends up calling
  // the exp_pi_diff variant for teams, which uses the loop bind(teams)
  // directive via metadirective selection.

  #pragma omp target teams map(from: d[0:N])
  exp_pi_diff(d,my_pi,N);
  // value should be near 1
  printf("d[N-1] = %20.14f\n",d[N-1]); // ...= 1.00000000000311

  // Case 2: exp_pi_diff called from a parallel region, so that
  // array is computed across team of threads. This ends up calling
  // the exp_pi_diff variant for parallel, which uses the loop
  // bind(parallel) directive via metadirective selection.

  #pragma omp parallel
  exp_pi_diff(d,my_pi,N);
  // value should be near 1
  printf("d[N-1] = %20.14f\n",d[N-1]); // ...= 1.00000000000311

  // Case 3: exp_pi_diff called from outside a teams or parallel
  // region, so that array is computed by the encountering thread. This
  // ends up calling the exp_pi_diff base function, which uses the
  // loop bind(thread) directive via metadirective selection.

  exp_pi_diff(d,my_pi,N);
  // value should be near 1
  printf("d[N-1] = %20.14f\n",d[N-1]); // ...= 1.00000000000311

  return 0;
}
