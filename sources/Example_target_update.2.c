/*
* @@name:	target_update.2c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.0
*/
extern void init(float *, float *, int);
extern int maybe_init_again(float *, int);
extern void output(float *, int);
void vec_mult(float *p, float *v1, float *v2, int N)
{
   int i;
   init(v1, v2, N);
   #pragma omp target data map(to: v1[:N], v2[:N]) map(from: p[0:N])
   {
      int changed;
      #pragma omp target
      #pragma omp parallel for
      for (i=0; i<N; i++)
        p[i] = v1[i] * v2[i];
      changed = maybe_init_again(v1,  N);
      #pragma omp target update if (changed) to(v1[:N])
      changed = maybe_init_again(v2,  N);
      #pragma omp target update if (changed) to(v2[:N])
      #pragma omp target
      #pragma omp parallel for
      for (i=0; i<N; i++)
        p[i] = p[i] + (v1[i] * v2[i]);
   }
   output(p, N);
}
