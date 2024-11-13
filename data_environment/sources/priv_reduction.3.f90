! @@name:	priv_reduction.3
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_6.0
program nest_red
  implicit none
  integer :: x

  x = 0
  ! parallel reduction on shared x
  !$omp parallel reduction(+: x) num_threads(strict: 4)
    !$omp do reduction(+: x)   ! reduction on private x
      do i = 1, 10
        x = x + 1
      end do
    !$omp end do
  !$omp end parallel
  print *, "x =", x            ! should print 40, with 4 threads
end program
