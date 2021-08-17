! @@name: affinity_display.2.f90
! @@type: F-free
! @@compilable: yes
! @@linkable: yes
! @@expect: success
! @@version: omp_5.0
program affinity_display

   use omp_lib
   implicit none
   character(len=0) :: null
   integer          :: n_sockets, socket_num, n_thrds_on_socket;

   call omp_set_nested(.true.)        ! or env var= OMP_NESTED=true
   call omp_set_max_active_levels(2)  ! or env var= OMP_MAX_ACTIVE_LEVELS=2

   n_sockets         = omp_get_num_places()
   n_thrds_on_socket = omp_get_place_num_procs(0)

    ! OMP_NUM_THREADS=2,4
    ! OMP_PLACES="{0,2,4,6},{1,3,5,7}"  #2 sockets; even/odd proc-ids
    ! OMP_AFFINITY_FORMAT=\
    !     "nest_level= %L, parent_thrd_num= %a, thrd_num= %n, thrd_affinity= %A"

   !$omp parallel num_threads(n_sockets) private(socket_num)

     socket_num = omp_get_place_num()

     if(socket_num==0) then
       write(*,'("LEVEL 1 AFFINITIES 1 thread/socket ",i0," sockets")')n_sockets
     endif

     call omp_display_affinity(null)  !not needed if OMP_DISPLAY_AFFINITY=TRUE

       ! OUTPUT:
       ! LEVEL 1 AFFINITIES 1 thread/socket, 2 sockets:
       ! nest_level= 1, parent_thrd_num= 0, thrd_num= 0, thrd_affinity= 0,2,4,6
       ! nest_level= 1, parent_thrd_num= 0, thrd_num= 1, thrd_affinity= 1,3,5,7

     call socket_work(socket_num, n_thrds_on_socket)

   !$omp end parallel

end program

subroutine socket_work(socket_num, n_thrds)
   use omp_lib
   implicit none
   integer :: socket_num, n_thrds
   character(len=0) :: null
 
   !$omp parallel num_threads(n_thrds)
 
      if(omp_get_thread_num()==0) then
      write(*,'("LEVEL 2 AFFINITIES, ",i0," threads on socket ",i0)') &
            n_thrds,socket_num
      endif
  
      call omp_display_affinity(null);  !not needed if OMP_DISPLAY_AFFINITY=TRUE
  
      ! OUTPUT:
      ! LEVEL 2 AFFINITIES, 4 threads on socket 0
      ! nest_level= 2, parent_thrd_num= 0, thrd_num= 0, thrd_affinity= 0
      ! nest_level= 2, parent_thrd_num= 0, thrd_num= 1, thrd_affinity= 2
      ! nest_level= 2, parent_thrd_num= 0, thrd_num= 2, thrd_affinity= 4
      ! nest_level= 2, parent_thrd_num= 0, thrd_num= 3, thrd_affinity= 6
  
      ! LEVEL 2 AFFINITIES, 4 thrds on socket 1
      ! nest_level= 2, parent_thrd_num= 1, thrd_num= 0, thrd_affinity= 1
      ! nest_level= 2, parent_thrd_num= 1, thrd_num= 1, thrd_affinity= 3
      ! nest_level= 2, parent_thrd_num= 1, thrd_num= 2, thrd_affinity= 5
      ! nest_level= 2, parent_thrd_num= 1, thrd_num= 3, thrd_affinity= 7
  
      ! ... Do Some work on Socket
  
   !$omp end parallel
  
end subroutine
