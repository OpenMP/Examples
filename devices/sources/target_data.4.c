/*
* @@name:	target_data.4c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.0
*/


void vec_mult(float*, float*, float*, int);

extern void init(float*, float*, int);
extern void output(float*, int);


void foo(float *p0, float *v1, float *v2, int N)
{
   init(v1, v2, N);

   #pragma omp target data map(to: v1[0:N], v2[:N]) map(from: p0[0:N])
   {
      vec_mult(p0, v1, v2, N);
   }

   output(p0, N);
}



void vec_mult(float *p1, float *v3, float *v4, int N)
{
   int i;
   #pragma omp target map(to: v3[0:N], v4[:N]) map(from: p1[0:N])
   #pragma omp parallel for
   for (i=0; i<N; i++)
   {
     p1[i] = v3[i] * v4[i];
   }
}

