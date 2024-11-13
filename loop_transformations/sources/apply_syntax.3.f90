! @@name:	apply_syntax.3
! @@type:	F-free
! @@operation:	compile
! @@expect:	success
! @@version:	omp_6.0
subroutine apply_complexarg(A)
   implicit none
   double precision :: A(100,100)
   integer :: i, j

   !$omp tile sizes(4,5)            &
   !$omp&   apply(grid(1): reverse) &
   !$omp&   apply(intratile(2): unroll)
   do i = 1, 100
      do j = 1, 100
         A(j,i) = A(j,i) + 1
      end do
   end do
end subroutine
