/*
* @@name:	declare_target.5c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.0
*/
#define N 10000
#define M 1024
#pragma omp declare target
float Q[N][N];
#pragma omp declare simd uniform(i) linear(k) notinbranch
float P(const int i, const int k)
{
  return Q[i][k] * Q[k][i];
}
#pragma omp end declare target

float accum(void)
{
  float tmp = 0.0;
  int i, k;
#pragma omp target map(tofrom: tmp)
#pragma omp parallel for reduction(+:tmp)
  for (i=0; i < N; i++) {
    float tmp1 = 0.0;
#pragma omp simd reduction(+:tmp1)
    for (k=0; k < M; k++) {
      tmp1 += P(i,k);
    }
    tmp += tmp1;
  }
  return tmp;
}

/* Note:  The variable tmp is now mapped with tofrom, for correct 
   execution with 4.5 (and pre-4.5) compliant compilers. See Devices Intro. 
 */
