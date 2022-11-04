! @@name:	tasking.12
! @@type:	F-free
! @@operation:	compile
! @@expect:	rt-error
! @@version:	omp_3.1
subroutine foo()
  integer :: x
  x = 2
!$omp task mergeable
  x = x + 1
!$omp end task
!$omp taskwait
  print *, x   ! prints 2 or 3
end subroutine
