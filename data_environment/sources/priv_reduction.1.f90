! @@name:	priv_reduction.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_6.0
function do_red(n, v) result(sum_v)
  implicit none
  integer :: n, v(*)
  integer :: sum_v      ! sum_v is private
  integer :: i

  sum_v = 0
  !$omp do reduction(+: sum_v)
  do i = 1, n
    sum_v = sum_v + v(i)
  end do
end function

program priv_red
  use :: omp_lib, only : omp_get_thread_num
  implicit none
  integer, parameter :: N = 100
  integer :: i, v(N), s_v
  integer, external :: do_red

  do i = 1, N
    v(i) = i - 1
  end do

  !$omp parallel private(s_v)
    s_v = do_red(N, v)
    print 10, omp_get_thread_num(), s_v
 10 format("myid ", i0, ": sum of v = ", i0)
  !$omp end parallel
end program
