/*
* @@name:	declare_target.4
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_5.1
*/
#define N 10000

#pragma omp begin declare target
  float Q[N][N];
  float Pfun(const int i, const int k) { return Q[i][k] * Q[k][i]; }
#pragma omp end declare target

float accum(int k)
{
    float tmp = 0.0;
    #pragma omp target update to(Q)
    #pragma omp target map(tofrom: tmp)
    #pragma omp parallel for reduction(+:tmp)
    for(int i=0; i < N; i++)
        tmp += Pfun(i,k);
    return tmp;
}

/* Note:  The variable tmp is now mapped with tofrom, for correct 
          execution with 4.5 (and pre-4.5) compliant compilers.
          See Devices Intro.
 */
