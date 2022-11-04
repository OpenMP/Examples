/*
* @@name:	lastprivate.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	pre_omp_3.0
*/
void lastpriv (int n, float *a, float *b)
{
  int i;

  #pragma omp parallel
  {
    #pragma omp for lastprivate(i)
    for (i=0; i<n-1; i++)
      a[i] = b[i] + b[i+1];
  }

  a[i]=b[i];      /* i == n-1 here */
}
