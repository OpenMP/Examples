! @@name:	priv_reduction.2
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_6.0
subroutine do_red(n, v, sum_v)
  implicit none
  integer :: n, v(*)
  integer :: sum_v
  integer :: i

  sum_v = 0          ! sum_v is private
  !$omp do reduction(original(private),+: sum_v)
  do i = 1, n
    sum_v = sum_v + v(i)
  end do
end subroutine

program priv_red
  use :: omp_lib, only : omp_get_thread_num
  implicit none
  integer, parameter :: N = 100
  integer :: i, v(N), s_v

  do i = 1, N
    v(i) = i - 1
  end do

  !$omp parallel private(s_v)
    call do_red(N, v, s_v)
    print 10, omp_get_thread_num(), s_v
 10 format("myid ", i0, ": sum of v = ", i0)
  !$omp end parallel
end program
