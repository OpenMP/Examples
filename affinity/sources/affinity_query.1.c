/*
* @@name:	affinity_query.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	no
* @@expect:	success
* @@version:	omp_4.5
*/
#include <stdio.h>
#include <omp.h>

void socket_init(int socket_num)
{
   int n_procs;

   n_procs = omp_get_place_num_procs(socket_num);
   #pragma omp parallel num_threads(n_procs) proc_bind(close)
   {
      printf("Reporting in from socket num, thread num:  %d %d\n", 
                                socket_num,omp_get_thread_num() );
   }
}

int main()
{
   int n_sockets, socket_num;

   omp_set_nested(1);              // or export OMP_NESTED=true
   omp_set_max_active_levels(2);   // or export OMP_MAX_ACTIVE_LEVELS=2

   n_sockets = omp_get_num_places();
   #pragma omp parallel num_threads(n_sockets) private(socket_num) \
                        proc_bind(spread)
   {
      socket_num = omp_get_place_num();
      socket_init(socket_num);
   }

   return 0;
}
