! @@name:	async_target.5
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_6.0
program async_target_nowait_arg
  implicit none
  integer :: x
  logical :: is_deferred
  real :: rand_no

  x = 2
  ! Determine if computation is deferred
  call  random_number(rand_no)
  is_deferred=mod(int(rand_no*10), 2) == 1

  !$omp target map(tofrom: x) nowait(is_deferred)
  call update(x)
  !$omp end target
  
  ! Perform other tasks in parallel while the target region is executing

  if (is_deferred) then
    !$omp taskwait  
  endif

  if (x == 6) then
    stop  "Passed"
  else
    error stop  "Failed"
  endif

contains

  subroutine update(num)
    integer, intent(inout) :: num
    !$omp declare target
    num = num * 3
  end subroutine update

end program async_target_nowait_arg
