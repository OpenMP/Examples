/*
* @@name: metadirective.2c
* @@type: C
* @@compilable: yes
* @@linkable: no
* @@expect: success
* @@version: omp_5.0
*/

#define N 100
#include <stdio.h>
#include <omp.h>

void work_on_chunk(int idev, int i);

int main()                    //Driver
{
   int i,idev;

   for (idev=0; idev<omp_get_num_devices(); idev++)
   {
      #pragma omp target device(idev)
      #pragma omp metadirective \
                  when( implementation={vendor(nvidia)}, device={arch("kepler")}: \
                        teams num_teams(512) thread_limit(32) )                   \
                  when( implementation={vendor(amd)},    device={arch("fiji"  )}: \
                        teams num_teams(512) thread_limit(64) )                   \
                  default(                                                        \
                        teams)
      #pragma omp distribute parallel for
      for (i=0; i<N; i++) work_on_chunk(idev,i);
   }
   return 0;
}

