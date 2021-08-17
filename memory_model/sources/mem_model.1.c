/*
* @@name:	mem_model.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	rt-error
* @@version:	omp_3.1
*/
#include <stdio.h>
#include <omp.h>

int main(){
  int x;

  x = 2;
  #pragma omp parallel num_threads(2) shared(x)
  {

    if (omp_get_thread_num() == 0) {
       #pragma omp atomic write
       x = 5;
    } else {
      int xval;
      #pragma omp atomic read
      xval = x;
    /* Print 1: xval can be 2 or 5 */
      printf("1: Thread# %d: x = %d\n", omp_get_thread_num(), xval);
    }

    #pragma omp barrier

    if (omp_get_thread_num() == 0) {
    /* Print 2 */
      printf("2: Thread# %d: x = %d\n", omp_get_thread_num(), x);
    } else {
    /* Print 3 */
      printf("3: Thread# %d: x = %d\n", omp_get_thread_num(), x);
    }
  }
  return 0;
}
