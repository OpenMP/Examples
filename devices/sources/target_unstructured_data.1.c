/*
 * @@name:        target-unstructured-data.1.c
 * @@type:        C
 * @@compilable:  yes
 * @@linkable:    no
 * @@expect:      success
 * @@version:     omp_4.5
 */
#include <stdlib.h>
typedef struct {
  double *A;
  int N;
} Matrix;

void init_matrix(Matrix *mat, int n)
{
  mat->A = (double *)malloc(n*sizeof(double));
  mat->N = n;
  #pragma omp target enter data map(alloc:mat->A[:n])
}

void free_matrix(Matrix *mat)
{
  #pragma omp target exit data map(delete:mat->A[:mat->N])
  mat->N = 0;
  free(mat->A);
  mat->A = NULL;
}
