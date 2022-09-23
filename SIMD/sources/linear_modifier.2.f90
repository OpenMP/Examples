! @@name:	linear_modifier.2
! @@type:	F-free
! @@compilable:	yes
! @@linkable:	yes
! @@expect:	success
! @@version:	omp_5.2
module m
   integer, parameter :: NN = 1023
   integer :: a(NN)

 contains
   subroutine add_one2(p, i)
   !$omp declare simd(add_one2) linear(p: ref) linear(i: uval)
   implicit none
   integer :: p
   integer, intent(in) :: i

   p = p + i
   end subroutine
end module

program main
   use m
   implicit none
   integer :: i, p

   do i = 1, NN
      a(i) = i
   end do

   p = 1
   !$omp simd linear(p)
   do i = 1, NN
      call add_one2(a(p), i)
      p = p + 1
   end do

   do i = 1, NN
      if (a(i) /= i*2) then
         print *, "failed"
         stop
      endif
   end do
   print *, "passed"
end program
