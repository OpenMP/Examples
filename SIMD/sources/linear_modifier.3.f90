! @@name:	linear_modifier.3
! @@type:	F-free
! @@operation:	run
! @@expect:	success
! @@version:	omp_5.2
module func_mod
contains
   real(8) function func(x, y, i)
   implicit none
!$omp declare simd(func) simdlen(4) uniform(x, y) linear(i:val,step(1))
      
      real(8), intent(in) :: x(*), y(*)
      integer, intent(in) :: i

      func = x(i) + y(i)

   end function func
end module func_mod

program main
   use func_mod
   implicit none
   integer, parameter :: n = 128
   real(8) :: x(n), y(n), z1(n), z2
   integer :: i, k

   do i=1, n
      x(i) = real(i, kind=8)
      y(i) = real(i*2, kind=8)
   enddo

   k = 1
!$omp simd linear(k)
   do i=1, n
      z1(i) = func(x, y, k)
      k = k + 1
   enddo
 
   do i=1, n
      z2 = real(i+i*2, kind=8)
      if (z1(i) /= z2) then
         print *, 'failed'
         stop
      endif
   enddo
   print *, 'passed'
end program main
