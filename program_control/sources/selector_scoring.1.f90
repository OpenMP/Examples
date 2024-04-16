! @@name:	selector_scoring.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.0
module subs
contains

   subroutine f(a,i)
      !$omp declare variant(fx1) match(construct={target})
      !$omp declare variant(fx2) match(construct={teams,parallel,do})
      !$omp declare variant(fx3) match(device={kind(gpu),isa(sm_70)})
      !$omp declare variant(fx4) match(device={arch(nvptx),isa(sm_70)})
      integer, intent(out) :: a
      integer, value :: i 
      a = i
   end subroutine

   subroutine fx1(a,i)
      integer, intent(out) :: a
      integer, value :: i 
      a = i
   end subroutine

   subroutine fx2(a,i)
      integer, intent(out) :: a
      integer, value :: i 
      a = 2*i
   end subroutine

   subroutine fx3(a,i)
      integer, intent(out) :: a
      integer, value :: i 
      a = 3*i
   end subroutine

   subroutine fx4(a,i)
      integer, intent(out) :: a
      integer, value :: i 
      a = 4*i
   end subroutine

end module subs


program main
   use subs
   integer, parameter :: N = 4
   integer :: a(N)
   integer :: i

   !$omp target teams distribute parallel do map(a)
   do i = 1, N
      !$omp task
         call f(a(i),i)
      !$omp end task
   end do

   do i = 1, N
      if (a(i) /= 4*i ) then
         print *, "Failed"
         error stop
      end if
   end do

   print *, "Passed"

end program
