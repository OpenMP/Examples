/*
* @@name:	target_data.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_4.0
*/
extern void init(float*, float*, int);
extern void output(float*, int);
void vec_mult(float *p, float *v1, float *v2, int N)
{
   int i;
   init(v1, v2, N);
   #pragma omp target data map(to: v1[0:N], v2[:N]) map(from: p[0:N])
   {
      #pragma omp target
      #pragma omp parallel for
      for (i=0; i<N; i++)
	p[i] = v1[i] * v2[i];
   }
   output(p, N);
}
