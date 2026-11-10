! @@name:	free_agent.2
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_6.0

! OMP_THREADS_RESERVE="structured(64),free_agent(4)"

program main
  use omp_lib
  implicit none

  integer :: count = 0
  integer :: val

  val = fib(20)
  print *, "fib(20) =", val
  print *, "Number of tasks executed by free-agent threads =", count

contains

recursive function fib(n) result(res)
    integer, intent(in) :: n
    integer :: i, j, res

    if (n < 2) then
       res = n
       return
    end if

    ! Create tasks for recursive Fibonacci calculation
    !$omp task shared(i) threadset(omp_pool)
    i = fib(n - 1)
    !$omp end task

    !$omp task shared(j) threadset(omp_pool)
    j = fib(n - 2)
    !$omp end task

    !$omp taskwait

    ! Count tasks executed by free agent threads
    if (omp_is_free_agent()) then
       !$omp atomic
       count = count + 1
    end if

    res = i + j
end function fib

end program main
  
