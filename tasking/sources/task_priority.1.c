/*
* @@name:       task_priority.1c
* @@type:       C
* @@compilable: yes
* @@linkable:   no
* @@expect:     success
* @@version:    omp_4.5
*/
void compute_array (float *node, int M);

void compute_matrix (float *array, int N, int M)
{
   int i;
   #pragma omp parallel private(i)
   #pragma omp single
   {
      for (i=0;i<N; i++) {
         #pragma omp task priority(i)
         compute_array(&array[i*M], M);
      }
   }
}
