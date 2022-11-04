! @@name:	linear_modifier.1
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.2
module m
   integer, parameter :: NN = 1023
   integer :: a(NN)

 contains
   subroutine add_one2(p)
   implicit none
   !$omp declare simd(add_one2) linear(p: ref) simdlen(8)
   
   integer :: p

   p = p + 1
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
   !$omp simd linear(p) simdlen(8)
   do i = 1, NN
      associate(k => a(p))
         call add_one2(k)
      end associate
      call add_one2(a(i))
      p = p + 1
   end do

   do i = 1, NN
      if (a(i) /= i+2) then
         print *, "failed"
         stop
      endif
   end do
   print *, "passed"
end program
