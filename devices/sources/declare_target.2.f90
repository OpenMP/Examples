! @@name:	declare_target.2
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	success
! @@version:	omp_4.0
program my_fib
integer :: N = 8
interface
  subroutine fib(N)
    !$omp declare target
    integer :: N
  end subroutine fib
end interface
   !$omp target
      call fib(N)
   !$omp end target
end program
subroutine fib(N)
integer :: N
!$omp declare target
     print*,"hello from fib"
     !...
end subroutine
