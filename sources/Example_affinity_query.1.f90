! @@name:	affinity_query.1f
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	no
! @@expect:	success
! @@version:	omp_4.5

subroutine socket_init(socket_num)
   use omp_lib
   integer  :: socket_num, n_procs

   n_procs = omp_get_place_num_procs(socket_num)
   !$omp parallel num_threads(n_procs) proc_bind(close)

      print*,"Reporting in from socket num, thread num: ",  &
                                socket_num,omp_get_thread_num()
   !$omp end parallel
end subroutine

program numa_teams
   use omp_lib
   integer :: n_sockets, socket_num

   call omp_set_nested(.true.)            ! or export OMP_NESTED=true
   call omp_set_max_active_levels(2) ! or export OMP_MAX_ACTIVE_LEVELS=2

   n_sockets = omp_get_num_places()
   !$omp parallel num_threads(n_sockets) private(socket_num) &
   !$omp&         proc_bind(spread)

      socket_num = omp_get_place_num()
      call socket_init(socket_num)

   !$omp end parallel
end program
