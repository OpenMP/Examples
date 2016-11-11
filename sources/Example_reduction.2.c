/*
* @@name:	reduction.2c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
*/
#include <limits.h>
#include <math.h>
void reduction2(float *x, int *y, int n)
{
  int i, b, b_p, c, c_p;
  float a, a_p, d, d_p;
  a = 0.0f;
  b = 0;
  c = y[0];
  d = x[0];
  #pragma omp parallel shared(a, b, c, d, x, y, n) \
                          private(a_p, b_p, c_p, d_p)
  {
    a_p = 0.0f;
    b_p = 0;
    c_p = INT_MAX;
    d_p = -HUGE_VALF;
    #pragma omp for private(i)
    for (i=0; i<n; i++) {
      a_p += x[i];
      b_p ^= y[i];
      if (c_p > y[i]) c_p = y[i];
      d_p = fmaxf(d_p,x[i]);
    }
    #pragma omp critical
    {
      a += a_p;
      b ^= b_p;
      if( c > c_p ) c = c_p;
      d = fmaxf(d,d_p);
    }
  }
}
