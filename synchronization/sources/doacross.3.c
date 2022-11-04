/*
* @@name:	doacross.3
* @@type:	C
* @@operation:	compile
* @@expect:	ct-error
* @@version:	omp_5.2
*/
#define N 100

void work_wrong(double p[][N][N])
{
  int i, j, k;

  #pragma omp parallel for ordered(2) private(i,j,k)
  for (i=1; i<N-1; i++)
  {
    for (j=1; j<N-1; j++)
    {
  #pragma omp ordered doacross(sink: i-1,j) doacross(sink: i+1,j) \
                      doacross(sink: i,j-1) doacross(sink: i,j+1)
      for (k=1; k<N-1; k++)
      {
        double tmp1 = p[i-1][j][k] + p[i+1][j][k];
        double tmp2 = p[i][j-1][k] + p[i][j+1][k];
        double tmp3 = p[i][j][k-1] + p[i][j][k+1];
        p[i][j][k] = (tmp1 + tmp2 + tmp3) / 6.0;
      }
/* missing #pragma omp ordered doacross(source:) */
    }
  }
}
