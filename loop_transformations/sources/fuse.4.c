/*
* @@name:	fuse.4
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.1
*/
float f(int,int);
float g(int,int);

void fuse(int n, float A[][n], float B[][n])
{
  #pragma omp fuse depth(2)
  {
    for (int i = 0; i < n; ++i)
      for (int k = 0; k < n; ++k)
        A[i][k] = f(i,k);
    for (int j = 0; j < n; ++j)
      for (int l = 0; l < n; ++l)
        B[j][l] = g(j,l);
  }
}

void fuse_midpoint(int n, float A[][n], float B[][n])
{
  for (int ij = 0; ij < n; ++ij) {
    #pragma omp fuse depth(1)
    {
      for (int k = 0; k < n; ++k)
        A[ij][k] = f(ij,k);
      for (int l = 0; l < n; ++l)
        B[ij][l] = g(ij,l);
    }
  }
}

void fuse_equivalent(int n, float A[][n], float B[][n])
{
  for (int ij = 0; ij < n; ++ij) {
    for (int kl = 0; kl < n; ++kl) {
      A[ij][kl] = f(ij,kl);
      B[ij][kl] = g(ij,kl);
    }
  }
}
