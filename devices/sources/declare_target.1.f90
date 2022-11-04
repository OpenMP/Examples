! @@name:	declare_target.1
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_4.0
module module_fib
integer :: THRESHOLD=1000000
contains
   subroutine fib(N)
      integer :: N
      !$omp declare target
      !...
   end subroutine
end module
subroutine my_fib(N)
use module_fib
integer :: N
   !$omp target if( N > THRESHOLD )
      call fib(N)
   !$omp end target
end subroutine
