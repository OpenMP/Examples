/*
* @@name:	free_agent.1
* @@type:	C
* @@operation:	compile
* @@expect:	success
* @@version:	omp_6.0
*/
extern void do_background_io();
extern void do_heavy_work();
#define N 100

int main()
{
  int i;
  // Create a task for background I/O
  #pragma omp task threadset(omp_pool)
    do_background_io();

  // Parallel loop for heavy work
  #pragma omp parallel for
   for (i = 0; i < N; i++)
     do_heavy_work();

  // Wait for tasks to complete
  #pragma omp taskwait

  // Use results from previous tasks

  return 0;
}
