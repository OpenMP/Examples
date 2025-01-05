/*
* @@name:	task_dep.14
* @@type:	C
* @@operation:	run
* @@expect:	success
* @@version:	omp_6.0
*/
#include <omp.h>

void my_func(int *M, int *v);

#define N_ROWS 20
#define N_COLS 20
#define NUM_VS 5
#define ROWS_PER_TASK 5
int M[N_ROWS*N_COLS], v[NUM_VS][N_COLS];

int main()
{
  for (int i = 0; i < N_ROWS*N_COLS; i++)
    M[i] = 1;

  for (int i = 0; i < NUM_VS; i++)
    for (int j = 0; j < N_COLS; j++)
      v[i][j] = 2;

  #pragma omp parallel single
  for (int h = 0;  h < NUM_VS; h++) {
    // Generate transparent task to establish dependences
    // between child tasks that don't share the same parent.
    #pragma omp task depend(inout:h) transparent(omp_impex)
    my_func(M, v[h]);
  }

  int check_value = 1;
  for (int i = 0; i < NUM_VS; i++)
    check_value *= 2;
  for (int i = 0; i < N_ROWS*N_COLS; i++)
    if (M[i] != check_value)
      return 1;

  return 0;
}

void my_func(int *M, int *v)
{
  for (int i = 0; i < N_ROWS; i += ROWS_PER_TASK) {
    // This task is dependency-ordered with respect to the corresponding
    // task in iteration i generated by other transparent tasks.
    #pragma omp task depend(inout:M[i*N_COLS])
    for (int j = 0; j < ROWS_PER_TASK; j++)
      for (int k = 0; k < N_COLS; k++)
        M[(i+j)*N_COLS + k] *= v[k];
  }
}