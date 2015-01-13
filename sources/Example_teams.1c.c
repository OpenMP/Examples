/*
* @@name:	teams.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/
#include <stdlib.h>
#include <omp.h>
float dotprod(float B[], float C[], int N)
{
   float sum0 = 0.0;
   float sum1 = 0.0;
   #pragma omp target map(to: B[:N], C[:N])
   #pragma omp teams num_teams(2)
   {
      int i;
      if (omp_get_num_teams() != 2)
         abort();
      if (omp_get_team_num() == 0)
      {
	 #pragma omp parallel for reduction(+:sum0)
	 for (i=0; i<N/2; i++)
	    sum0 += B[i] * C[i];
      }
      else if (omp_get_team_num() == 1)
      {
	 #pragma omp parallel for reduction(+:sum1)
	 for (i=N/2; i<N; i++)
	    sum1 += B[i] * C[i];
      }
   }
   return sum0 + sum1;
}
