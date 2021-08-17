/*
* @@name:       affinity_display.2c
* @@type:       C
* @@compilable: yes
* @@linkable:   yes
* @@expect:     success
* @@version:	omp_5.0
*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void socket_work(int socket_num, int n_thrds);

int main(void)
{
 int n_sockets, socket_num, n_thrds_on_socket;

 omp_set_nested(1);            // or env var= OMP_NESTED=true
 omp_set_max_active_levels(2); // or env var= OMP_MAX_ACTIVE_LEVELS=2

  n_sockets         = omp_get_num_places();
  n_thrds_on_socket = omp_get_place_num_procs(0);

     // OMP_NUM_THREADS=2,4
     // OMP_PLACES="{0,2,4,6},{1,3,5,7}"  #2 sockets; even/odd proc-ids
     // OMP_AFFINITY_FORMAT=\
     // "nest_level= %L, parent_thrd_num= %a, thrd_num= %n, thrd_affinity= %A"
    
  #pragma omp parallel num_threads(n_sockets) private(socket_num)
  {
    socket_num = omp_get_place_num();

    if(socket_num==0) 
       printf(" LEVEL 1 AFFINITIES 1 thread/socket, %d sockets:\n\n", n_sockets);

    omp_display_affinity(NULL);   // not needed if OMP_DISPLAY_AFFINITY=TRUE

     // OUTPUT:
     // LEVEL 1 AFFINITIES 1 thread/socket, 2 sockets:
     // nest_level= 1, parent_thrd_num= 0, thrd_num= 0, thrd_affinity= 0,2,4,6
     // nest_level= 1, parent_thrd_num= 0, thrd_num= 1, thrd_affinity= 1,3,5,7

    socket_work(socket_num, n_thrds_on_socket);
  }
  
  return 0;
}

void socket_work(int socket_num, int n_thrds)
{  
  #pragma omp parallel num_threads(n_thrds)
  {
    if(omp_get_thread_num()==0) 
  printf(" LEVEL 2 AFFINITIES, %d threads on socket %d\n",n_thrds, socket_num);
        
    omp_display_affinity(NULL);   // not needed if OMP_DISPLAY_AFFINITY=TRUE
         
    // OUTPUT:
    // LEVEL 2 AFFINITIES, 4 threads on socket 0
    // nest_level= 2, parent_thrd_num= 0, thrd_num= 0, thrd_affinity= 0
    // nest_level= 2, parent_thrd_num= 0, thrd_num= 1, thrd_affinity= 2
    // nest_level= 2, parent_thrd_num= 0, thrd_num= 2, thrd_affinity= 4
    // nest_level= 2, parent_thrd_num= 0, thrd_num= 3, thrd_affinity= 6

    // LEVEL 2 AFFINITIES, 4 threads on socket 1
    // nest_level= 2, parent_thrd_num= 1, thrd_num= 0, thrd_affinity= 1
    // nest_level= 2, parent_thrd_num= 1, thrd_num= 1, thrd_affinity= 3
    // nest_level= 2, parent_thrd_num= 1, thrd_num= 2, thrd_affinity= 5
    // nest_level= 2, parent_thrd_num= 1, thrd_num= 3, thrd_affinity= 7
         
    // ... Do Some work on Socket
         
  }
}    
