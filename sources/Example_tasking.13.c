/*
* @@name:	tasking.13c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_3.1
*/
#include <string.h>
#include <omp.h>
#define LIMIT  3 /* arbitrary limit on recursion depth */
void check_solution(char *);
void bin_search (int pos, int n, char *state)
{
   if ( pos == n ) {
      check_solution(state);
      return;
   }
   #pragma omp task final( pos > LIMIT ) mergeable
   {
      char new_state[n];
      if (!omp_in_final() ) {
        memcpy(new_state, state, pos );
        state = new_state;
      }
      state[pos] = 0;
      bin_search(pos+1, n, state );
   }
   #pragma omp task final( pos > LIMIT ) mergeable
   {
      char new_state[n];
      if (! omp_in_final() ) {
        memcpy(new_state, state, pos );
        state = new_state;
      }
      state[pos] = 1;
      bin_search(pos+1, n, state );
   }
   #pragma omp taskwait
}
