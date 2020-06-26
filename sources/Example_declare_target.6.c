/*
* @@name: declare_target.6.c
* @@type: C
* @@compilable: yes
* @@linkable: no
* @@expect: success
* @@version: omp_4.5
*/
#define N 100000000

float  sp[N], sv1[N], sv2[N];
double dp[N], dv1[N], dv2[N];
#pragma omp declare target link(sp,sv1,sv2) \
                           link(dp,dv1,dv2)

void s_init(float *, float *, int);
void d_init(double *, double *, int);
void s_output(float *, int);
void d_output(double *, int);

#pragma omp declare target
void s_vec_mult_accum()
{
   int i;

   #pragma omp parallel for
   for (i=0; i<N; i++)
     sp[i] = sv1[i] * sv2[i];
}

void d_vec_mult_accum()
{
   int i;

   #pragma omp parallel for
   for (i=0; i<N; i++)
     dp[i] = dv1[i] * dv2[i];
}
#pragma omp end declare target

int main()
{
   s_init(sv1, sv2, N);
   #pragma omp target map(to:sv1,sv2) map(from:sp)
      s_vec_mult_accum();
   s_output(sp, N);

   d_init(dv1, dv2, N);
   #pragma omp target map(to:dv1,dv2) map(from:dp)
      d_vec_mult_accum();
   d_output(dp, N);

  return 0;
}
