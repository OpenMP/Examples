! @@name:	free_agent.3
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_6.0

! OMP_THREADS_RESERVE="structured(2),free_agent(2)"

program openmp_taskloop_example
  use omp_lib
  implicit none

  integer :: i
  integer :: count = 0

  ! Parallel region with strict 2 threads
  !$omp parallel masked num_threads(strict: 2)

  ! Taskloop with strict grainsize of 1
  !$omp taskloop grainsize(strict: 1) threadset(omp_pool)
  do i = 1,40
     if (omp_is_free_agent()) then
        !$omp atomic
        count = count + 1
     end if
  end do
  !$omp end taskloop

  !$omp end parallel masked

  ! Print the result
  print *, "Number of tasks executed by free-agent threads =", count

end program openmp_taskloop_example

