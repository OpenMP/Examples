/*
* @@name:	mem_model.2c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	rt-error
* @@version:	omp_3.1
*/
#include <omp.h>
#include <stdio.h>
int main()
{
    int data;
    int flag=0;
    #pragma omp parallel num_threads(2)
    {
      if (omp_get_thread_num()==0)
      {
      /* Write to the data buffer that will be
       * read by thread */
          data = 42;
      /* Flush data to thread 1 and strictly order
       * the write to data relative to the write to the flag */
          #pragma omp flush(flag, data)
      /* Set flag to release thread 1 */
          #pragma omp atomic write
          flag = 1;
      }
      else if(omp_get_thread_num()==1)
      {
      /* Loop until we see the update to the flag */
          #pragma omp flush(flag, data)
          int flag_val = 0;
          while (flag_val < 1)
          {
             #pragma omp atomic read
             flag_val = flag;
          }
      /* Value of flag is 1; value of data is undefined */
          printf("flag=%d data=%d\n", flag, data);
          #pragma omp flush(flag, data)
      /* Value of flag is 1; value of data is 42 */
          printf("flag=%d data=%d\n", flag, data);
      }
    }
    return 0;
}
