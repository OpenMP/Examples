! @@name:	SIMD.1
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_4.0
subroutine star(a,b,c,n,ioff_ptr)
   implicit none
   double precision :: a(*),b(*),c(*)
   integer          :: n, i
   integer, pointer :: ioff_ptr
 
   !$omp simd
   do i = 1,n
      a(i) = a(i) * b(i) * c(i+ioff_ptr)
   end do
 
end subroutine
