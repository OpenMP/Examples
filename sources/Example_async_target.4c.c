/*
* @@name:       target.4c
* @@type:       C
* @@compilable: yes
* @@linkable:   no
* @@expect:     success
*/

extern void init(  float*, int);
extern void output(float*, int);
void vec_mult(int N)
{
   int i;
   float p[N], v1[N], v2[N];

   #pragma omp parallel num_threads(2)
   {
      #pragma omp single
      {
         #pragma omp task depend(out:v1)
         init(v1, N);

         #pragma omp task depend(out:v2)
         init(v2, N);

         #pragma omp target nowait depend(in:v1,v2) depend(out:p) map(from: p) map(to:v1,v2)
         #pragma omp parallel for private(i)
         for (i=0; i<N; i++)
            p[i] = v1[i] * v2[i];

         #pragma omp task depend(in:p)
         output(p, N);
      }
   }
}
