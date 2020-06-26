/*
* @@name: affinity.1.c
* @@type: C
* @@compilable: yes
* @@linkable: no
* @@expect: success
* @@version: omp_5.0
*/

double * alloc_init_B(double *A, int N);
void     compute_on_B(double *B, int N);

void task_affinity(double *A, int N)
{
   double * B;
   #pragma omp task depend(out:B) shared(B) affinity(A[0:N])
   {
     B = alloc_init_B(A,N);
   }

   #pragma omp task depend( in:B) shared(B) affinity(A[0:N])
   {
     compute_on_B(B,N);
   }

   #pragma omp taskwait
}

