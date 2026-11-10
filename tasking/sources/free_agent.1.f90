! @@name:	free_agent.1
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_6.0
program openmp_task_example
  implicit none

  integer :: i
  integer, parameter :: N = 100

  ! Declare external procedures
  external :: do_background_io
  external :: do_heavy_work


  ! Create a task for background I/O
  !$omp task threadset(omp_pool)
  call do_background_io()
  !$omp end task

  ! Parallel loop for heavy work
  !$omp parallel do
  do i = 1, N
     call do_heavy_work()
  end do
  !$omp end parallel do

  ! Wait for tasks to complete
  !$omp taskwait

  ! Use results from previous tasks

end program openmp_task_example
