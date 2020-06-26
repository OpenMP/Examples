/*
* @@name:	teams.2c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.0
*/
#define min(x, y) (((x) < (y)) ? (x) : (y))

float dotprod(float B[], float C[], int N, int block_size,
  int num_teams, int block_threads)
{
    float sum = 0.0;
    int i, i0;
    #pragma omp target map(to: B[0:N], C[0:N]) map(tofrom: sum)
    #pragma omp teams num_teams(num_teams) thread_limit(block_threads) \
      reduction(+:sum)
    #pragma omp distribute
    for (i0=0; i0<N; i0 += block_size)
       #pragma omp parallel for reduction(+:sum)
       for (i=i0; i< min(i0+block_size,N); i++)
           sum += B[i] * C[i];
    return sum;
}

/* Note:  The variable sum is now mapped with tofrom, for correct 
   execution with 4.5 (and pre-4.5) compliant compilers. See Devices Intro. 
 */
