/*
* @@name:	get_wtime.1
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
*/
#include <stdio.h>
#include <unistd.h>
#include <omp.h>

void work_to_be_timed()
{
  sleep(2);
}

int main()
{
  double start, end;

  start = omp_get_wtime();
  work_to_be_timed();     // any parallel or serial codes
  end = omp_get_wtime();

  printf("Work took %f seconds\n", end - start);
  printf("Precision of the timer is %f (sec)\n", omp_get_wtick());
  return 0;
}
